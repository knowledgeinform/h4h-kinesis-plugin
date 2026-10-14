/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#include "agent/h4h_vehicle.h"
#include "zmq.hpp"
#include "sdk/notification.h"
#include <json.hpp>
#include <regex>
using json = nlohmann::json;

#include <cmath>
#include <iomanip>

namespace kinesis_example {

bool DemoH4HVehicle::constructNode(const std::string& name, const std::string& type, const std::string& controlPort, const std::string& statusPort, const std::string& camURL)
{
  kinesis::NodeFactory factory(kIntegratorId);
  auto vehicle = std::make_shared<DemoH4HVehicle>(name, type, controlPort, statusPort, camURL);
  factory.addPlatform(vehicle->getPlatform());
  factory.addActions(vehicle->getActions());

    for (const auto& view : vehicle->views_)
    {
        factory.addView(view->getViewInterface());
    }

  return factory.constructNode(vehicle) != kinesis::NodeFactory::kConstructionFailure;
}

DemoH4HVehicle::DemoH4HVehicle(const std::string& node_name, const std::string& type, const std::string& controlPort, const std::string& statusPort, const std::string& camURL)
  : name_(node_name)
  , type_(type)
  , controlPort_(controlPort)
  , statusPort_(statusPort)
  , camURL_(camURL)
  , last_lin_msg_time(std::chrono::steady_clock::now())
  , last_ang_msg_time(std::chrono::steady_clock::now())
  , location_(28.073230, -80.60103, 0.0, kinesis::Location::AltitudeReference::kTakeoff)
{
  if (type_ == "UAV") {
    location_.altitude_reference = kinesis::Location::AltitudeReference::kTakeoff;
    vehicle_state_ = VehicleState::kLanded;
  }

  // ZMQ setup
  zmq_context = zmq::context_t{1};
  mobility_socket = zmq::socket_t{zmq_context, zmq::socket_type::pub};
  std::string connection = "tcp://*:" + controlPort_ ;

  mobility_socket.bind(connection);

//  std::regex pattern(R"((\d{1,3}(\.\d{1,3}){3}\:\d{4}))");
  std::regex pattern(R"(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}:\d{1,5})");
  if (std::regex_match(statusPort_, pattern)) {
//    energy_ratio_ = 0.5;
    std::string status_connection = "tcp://" + statusPort_;
    std::string stat_str = "status connection string: " + status_connection ;

    status_socket = zmq::socket_t(zmq_context, zmq::socket_type::sub);
    status_socket.set(zmq::sockopt::conflate, true);
    status_socket.set(zmq::sockopt::subscribe, "");
    status_socket.connect(status_connection);
    feedback_ = true;
  }

  // add view
    if (!camURL_.empty()) {
        views_.push_back(std::make_shared<DemoView>("PlatformView",
                                                    camURL_,
                                                    kinesis::ViewInterface::SensorType::kVisible,
                                                    kinesis::ViewInterface::FieldOfView(90.0, 70.0),
                                                    kinesis::FrameInterface::kIdPlatform,       // parent ID
                                                    kinesis::Vector(),                          // linear offset
                                                    kinesis::Quaternion::getRotationY(M_PI_2),  // angular offset
                                                    0.0,                                        // no zoom
                                                    false,                                      // don't crop
                                                    true,   // single image capture supported
                                                    5.0));
    }

}

DemoH4HVehicle::~DemoH4HVehicle()
{
}

void DemoH4HVehicle::initialize()
{
  kLog.v << "Initialize Called";
}

    void DemoH4HVehicle::update()
    {
        const auto update_time = std::chrono::steady_clock::now();

        const long double seconds_since_last_update =
                std::chrono::duration_cast<std::chrono::milliseconds>(
                        update_time - time_update_).count() /
                1000.0;

        time_update_ = update_time;

        /*
         * Process vehicle status.
         *
         * Status telemetry is authoritative. Arm/disarm button callbacks only send
         * requests; they do not directly change vehicle_state_.
         */
        if (feedback_)
        {
            zmq::message_t msg;

            try
            {
                const auto receive_result =
                        status_socket.recv(msg, zmq::recv_flags::dontwait);

                if (receive_result)
                {
                    status_recv = true;
                    energy_ratio_ = 0.5;

                    const json from_uas = json::parse(msg.to_string());

                    if (from_uas.value("messageType", "") == "status")
                    {
                        const json message_data =
                                from_uas.value("messageData", json::object());

                        location_.latitude =
                                message_data.value("lat", location_.latitude);

                        location_.longitude =
                                message_data.value("lon", location_.longitude);

                        location_.altitude =
                                message_data.value("alt", location_.altitude);

                        energy_ratio_ =
                                message_data.value("powerLevel", energy_ratio_);

                        if (type_ == "UAV")
                        {
                            /*
                             * Armed telemetry controls the state transition:
                             *
                             * armed == true:
                             *   kLanded/kTeleop -> kFlying
                             *
                             * armed == false:
                             *   any state -> kLanded
                             *
                             * A failed arm therefore remains kLanded.
                             * A failed disarm remains kFlying or kAutonomy.
                             */
                            if (message_data.contains("armed"))
                            {
                                if (message_data["armed"].is_boolean())
                                {
                                    updateArmStateFromTelemetry(
                                            message_data["armed"].get<bool>());
                                }
                                else
                                {
                                    kLog.w << "Ignoring non-boolean armed status field";
                                }
                            }

                            /*
                             * Preserve the current mode when flight_mode is missing.
                             */
                            const std::string current_uav_mode =
                                    getUavMode(uav_mode);

                            const std::string incoming_uav_mode =
                                    message_data.value(
                                            "flight_mode",
                                            current_uav_mode);

                            if (incoming_uav_mode != current_uav_mode)
                            {
                                auto notification =
                                        kinesis::Notification::build(
                                                kinesis::Notification::Severity::kStatus,
                                                name_,
                                                name_ + " - Flight Mode Changed");

                                notification->setDescription(
                                        "New Flight Mode: " + incoming_uav_mode);

                                notification->broadcast();
                            }

                            if (incoming_uav_mode == "HOLD")
                            {
                                uav_mode = UavFlightMode::kHold;
                            }
                            else if (incoming_uav_mode == "MANUAL")
                            {
                                uav_mode = UavFlightMode::kManual;
                            }
                            else if (incoming_uav_mode == "POSITION")
                            {
                                uav_mode = UavFlightMode::kPosition;
                            }
                            else if (incoming_uav_mode == "OFFBOARD")
                            {
                                uav_mode = UavFlightMode::kOffboard;
                            }
                            else if (incoming_uav_mode != current_uav_mode)
                            {
                                kLog.w << "Unknown UAV flight mode received: "
                                       << incoming_uav_mode;
                            }
                        }
                    }
                }
            }
            catch (const json::exception& e)
            {
                kLog.e << "Failed to process status message JSON: "
                       << e.what();
            }
            catch (const zmq::error_t& e)
            {
                kLog.e << "Failed to receive status message: "
                       << e.what();
            }
            catch (const std::exception& e)
            {
                kLog.e << "Failed to process status message: "
                       << e.what();
            }
        }

        /*
         * Simulate location and orientation only until real status has been received.
         */
        if (!status_recv)
        {
            const kinesis::Vector displacement =
                    linear_velocity_ * seconds_since_last_update;

            location_ =
                    location_.getRelativeLocation(
                            displacement.x,
                            displacement.y,
                            displacement.z);

            yaw_ += angular_velocity_.z * seconds_since_last_update;

            if (yaw_ > M_PI)
            {
                yaw_ -= M_PI * 2.0;
            }
            else if (yaw_ < -M_PI)
            {
                yaw_ += M_PI * 2.0;
            }

            attitude_ =
                    kinesis::Quaternion::getRotationZ(yaw_);
        }

        /*
         * Do not call startArm() or stopArm() here.
         *
         * Those are operator-action callbacks. Calling them every update would
         * continuously generate commands and notifications.
         */
        switch (vehicle_state_)
        {
            case VehicleState::kLanded:
                linear_velocity_ =
                        kinesis::Vector(0.0, 0.0, 0.0);

                angular_velocity_ =
                        kinesis::Vector(0.0, 0.0, 0.0);
                break;

            case VehicleState::kFlying:
                break;

            case VehicleState::kAutonomy:
                linear_velocity_ =
                        kinesis::Vector(0.0, 0.0, 0.0);
                break;

            case VehicleState::kTeleop:
                break;
        }

        signal_ratio_ = 1.0;

        /*
         * Movement control message.
         */
        if (send_zmq)
        {
            const auto now = std::chrono::steady_clock::now();

            const double time_since_last_message_ms =
                    std::chrono::duration<double, std::milli>(
                            now - last_lin_msg_time).count();

            if (time_since_last_message_ms >
                (1000.0 / zmqSendRateHz))
            {
                const json data = {
                        {"name", name_},
                        {"linear_x", linear_velocity_.x},
                        {"linear_y", -linear_velocity_.y},
                        {"linear_z", linear_velocity_.z},
                        {"angular_z", -angular_velocity_.z}
                };

                const json msgOut = {
                        {"messageType", "control"},
                        {"messageData", data}
                };

                try
                {
                    mobility_socket.send(
                            zmq::buffer(msgOut.dump()),
                            zmq::send_flags::none);

                    last_lin_msg_time = now;
                    send_zmq = false;
                }
                catch (const zmq::error_t& e)
                {
                    kLog.e << "Failed to send control message: "
                           << e.what();
                }
            }
        }

        /*
         * Action message.
         */
        if (send_zmq_action)
        {
            const auto now = std::chrono::steady_clock::now();

            const double time_since_last_message_ms =
                    std::chrono::duration<double, std::milli>(
                            now - last_ang_msg_time).count();

            if (time_since_last_message_ms >
                (1000.0 / zmqSendRateHz))
            {
                json data = {
                        {"name", name_}
                };

                if (type_ == "UAV")
                {
                    const json uav = {
                            {"arm", isArmCommandQueued()},
                            {"disarm", isDisarmCommandQueued()},
                            {"autonomy",
                                    vehicle_state_ == VehicleState::kAutonomy},
                            {"flight_mode",
                                    uav_mode_request.empty()
                                    ? getUavMode(uav_mode)
                                    : uav_mode_request},
                            {"sender", sender}
                    };

                    data.update(uav);
                }
                else
                {
                    const json actions = {
                            {"autonomy",
                             vehicle_state_ == VehicleState::kAutonomy}
                    };

                    data.update(actions);
                }

                const json msgOut = {
                        {"messageType", "actions"},
                        {"messageData", data}
                };

                kLog.i << "SENDING ACTION MESSAGE"
                       << ", sender=" << sender
                       << ", pending_arm_command="
                       << static_cast<int>(pending_arm_command_)
                       << ", telemetry_armed="
                       << (telemetry_armed_ ? "true" : "false")
                       << ", vehicle_state="
                       << static_cast<int>(vehicle_state_)
                       << ", payload="
                       << msgOut.dump();

                try
                {
                    mobility_socket.send(
                            zmq::buffer(msgOut.dump()),
                            zmq::send_flags::none);

                    // Don't clear pending_arm_command_ here if
                    // updateArmStateFromTelemetry() will clear it.

                    last_ang_msg_time = now;
                    send_zmq_action = false;
                }
                catch (const zmq::error_t& e)
                {
                    kLog.e << "Failed to send actions message: "
                           << e.what();
                }
            }
        }

        /*
         * Parameter message.
         */
        if (send_zmq_param)
        {
            const auto now = std::chrono::steady_clock::now();

            const double time_since_last_message_ms =
                    std::chrono::duration<double, std::milli>(
                            now - last_ang_msg_time).count();

            if (time_since_last_message_ms >
                (1000.0 / zmqSendRateHz))
            {
                json data = {
                        {"name", name_}
                };

                if (type_ == "UAV")
                {
                    const json uav = {
                            {"failsafe", failsafe_parameter},
                            {"rtl_altitude", rtl_altitude},
                            {"max_altitude", max_altitude}
                    };

                    data.update(uav);
                }

                const json msgOut = {
                        {"messageType", "params"},
                        {"messageData", data}
                };

                try
                {
                    mobility_socket.send(
                            zmq::buffer(msgOut.dump()),
                            zmq::send_flags::none);

                    last_ang_msg_time = now;
                    send_zmq_param = false;
                }
                catch (const zmq::error_t& e)
                {
                    kLog.e << "Failed to send parameter message: "
                           << e.what();
                }
            }
        }
    }

void DemoH4HVehicle::shutdown()
{
  kLog.v << "Shutdown Called";
}

kinesis::Location DemoH4HVehicle::getLocation() const
{
  return location_;
}

double DemoH4HVehicle::getEnergyRatio() const
{
  return energy_ratio_;
}

double DemoH4HVehicle::getSignalRatio() const
{
  return signal_ratio_;
}

kinesis::ParameterInterface DemoH4HVehicle::getParameters() const
{
  std::string display_name = kParameterDisplayName;
  kinesis::ParameterInterface parameters;
//  parameters.addParameterBool(display_name, toggle_parameter_);
  if (type_ == "UAV") {
    parameters.addParameterOption(kFailSafeParamDisplayName, failsafe_parameter, kFailSafeOptions);
    parameters.addParameterDouble(kRtlAltitudeParamDisplayName, rtl_altitude);
    parameters.addParameterDouble(kMaxAltitudeParamDisplayName, max_altitude);
    parameters.addParameterOption(kUavModeDisplayName, getUavMode(uav_mode), kUavModeOptions);
  }
  return parameters;
}

void DemoH4HVehicle::changeParameters(const kinesis::ParameterInterface& changed_parameters)
{
  kLog.v << "changeParameters() called with: " << changed_parameters.dumpAsString();
  // update the toggle_parameter_ value based on changed parameters call (if necessary)
  std::vector<std::string> display_names = changed_parameters.getDisplayNames();
  for (auto& name : display_names)
  {
    if (name == kFailSafeParamDisplayName || name == kMaxAltitudeParamDisplayName || name == kRtlAltitudeParamDisplayName) {
      failsafe_parameter = changed_parameters.getParameterOption(kFailSafeParamDisplayName)->selected_option;
      max_altitude = changed_parameters.getParameterDouble(kMaxAltitudeParamDisplayName).value_or(max_altitude);
      rtl_altitude = changed_parameters.getParameterDouble(kRtlAltitudeParamDisplayName).value_or(rtl_altitude);
      sender = "onchange_param";
      send_zmq_param = true;
    }
    else if (name == kUavModeDisplayName) {
      uav_mode_request = changed_parameters.getParameterOption(kUavModeDisplayName)->selected_option;
      if (uav_mode_request == getUavMode(uav_mode)) {
        return;
      }
      sender = "new_mode";
      send_zmq_action = true;
    }
  }
}

kinesis::Quaternion DemoH4HVehicle::getAngularPosition()
{
  return attitude_;
}

kinesis::Vector DemoH4HVehicle::getLinearVelocity() const
{
  return linear_velocity_;
}

void DemoH4HVehicle::setLinearVelocity(kinesis::Vector velocity)
{
  //  kLog.v << std::fixed << std::setprecision(2) << "setLinearVelocity " << velocity;
  linear_velocity_.x = (std::clamp(velocity.x, -kVelocityLimitLinearXY, kVelocityLimitLinearXY))/kNormalizeVelXY;
  linear_velocity_.y = (std::clamp(velocity.y, -kVelocityLimitLinearXY, kVelocityLimitLinearXY))/kNormalizeVelXY;
  linear_velocity_.z = (std::clamp(velocity.z, -kVelocityLimitLinearZ, kVelocityLimitLinearZ))/kNormalizeVelZ;
  sender = "linear_vel";
  send_zmq = true;
  }

void DemoH4HVehicle::setAngularVelocity(kinesis::Vector velocity)
{
  //  kLog.v << std::fixed << std::setprecision(2) << "setAngularVelocity " << velocity;
  angular_velocity_.z = (std::clamp(velocity.z, -kVelocityLimitAngularZ, kVelocityLimitAngularZ))/kNormalizeVelAng;

  sender = "angular_vel";
  send_zmq = true;
  }

//void DemoH4HVehicle::updateArmStateFromTelemetry(bool armed)
//{
//    const bool armed_state_changed =
//            telemetry_armed_ != armed;
//
//    telemetry_armed_ = armed;
//
//    if (!telemetry_armed_)
//    {
//        /*
//         * UAV reports disarmed.
//         *
//         * This covers:
//         * - successful disarm
//         * - failed arm
//         * - external disarm
//         * - failsafe landing followed by disarm
//         */
//        vehicle_state_ = VehicleState::kLanded;
//
//        linear_velocity_ =
//                kinesis::Vector(0.0, 0.0, 0.0);
//
//        angular_velocity_ =
//                kinesis::Vector(0.0, 0.0, 0.0);
//
////        also add changed_flag here to resend zmq status
////        if (armed_state_changed) {
////          send_zmq_action = true
////        }
//    }
//    // else if (telemetry_armed_) { // add flag to do if (telemetry_armed_) as sanity check
//      // add flag change for pending arm command
////        if (armed_state_changed) {
////                pending_arm_command_ = False
////                vehicle_state_ = VehicleState::kFlying;
////                send_zmq_action = true
//    else if (vehicle_state_ == VehicleState::kLanded ||
//             vehicle_state_ == VehicleState::kTeleop)
//    {
//        /*
//         * UAV reports armed.
//         *
//         * Do not overwrite kAutonomy while the UAV remains armed.
//         */
//        vehicle_state_ = VehicleState::kFlying;
//    }
//
//    if (armed_state_changed)
//    {
//      // don't need this anymore, can do in logic above (only want to send_zmq when pending request is done)
//      send_zmq_action = true;
//        kLog.i << "UAV armed telemetry changed to "
//               << (telemetry_armed_ ? "true" : "false");
//    }
//}

void DemoH4HVehicle::updateArmStateFromTelemetry(bool armed)
{
    const bool armed_state_changed =
            telemetry_armed_ != armed;

    telemetry_armed_ = armed;

    if (!telemetry_armed_)
    {
        /*
         * UAV reports disarmed.
         *
         * This covers:
         * - successful disarm
         * - failed arm
         * - external disarm
         * - failsafe landing followed by disarm
         */
        vehicle_state_ = VehicleState::kLanded;

        linear_velocity_ =
                kinesis::Vector(0.0, 0.0, 0.0);

        angular_velocity_ =
                kinesis::Vector(0.0, 0.0, 0.0);

        /*
         * Confirm a pending disarm request only when telemetry changes
         * from armed to disarmed.
         */
        if (armed_state_changed)
        {
            pending_arm_command_ = ArmCommand::kNone;

            /*
             * Resend the full action/status message after the pending
             * disarm request has been confirmed.
             */
            sender = "disarm_confirmed";
            send_zmq_action = true;

            kLog.i << "Pending disarm request confirmed by UAV telemetry";
        }
    }
    else if (telemetry_armed_)
    {
        /*
         * UAV reports armed.
         *
         * The explicit telemetry_armed_ check acts as a sanity check.
         * Do not overwrite kAutonomy while the UAV remains armed.
         */
        if (vehicle_state_ == VehicleState::kLanded ||
            vehicle_state_ == VehicleState::kTeleop)
        {
            vehicle_state_ = VehicleState::kFlying;
        }

        /*
         * Confirm a pending arm request only when telemetry changes
         * from disarmed to armed.
         */
        if (armed_state_changed)
        {
            pending_arm_command_ = ArmCommand::kNone;

            /*
             * Resend the full action/status message after the pending
             * arm request has been confirmed.
             */
            sender = "arm_confirmed";

            kLog.i << "Pending arm request confirmed by UAV telemetry";
        }
    }

    /*
     * Do not set send_zmq_action here for every telemetry change.
     * It is now set only when a pending request is confirmed above.
     */
    if (armed_state_changed)
    {
        kLog.i << "UAV armed telemetry changed to "
               << (telemetry_armed_ ? "true" : "false");
    }
}

void DemoH4HVehicle::queueArmCommand(ArmCommand command)
{
    if (type_ != "UAV")
    {
        return;
    }

    if (command == ArmCommand::kArm)
    {
        /*
         * Do not send an arm request when telemetry already reports armed.
         */
        if (telemetry_armed_)
        {
            return;
        }

        sender = "startArm";
        kLog.i << "Queueing one-shot arm request";
    }
    else if (command == ArmCommand::kDisarm)
    {
        /*
         * Do not send a disarm request when telemetry already reports disarmed.
         */
        if (!telemetry_armed_)
        {
            return;
        }

        sender = "stopArm";
        kLog.i << "Queueing one-shot disarm request";
    }
    else
    {
        return;
    }

    pending_arm_command_ = command;

    kLog.i << "ACTION SEND TRIGGER: queueArmCommand"
           << ", sender=" << sender
           << ", arm=" << (isArmCommandQueued() ? "true" : "false")
           << ", disarm=" << (isDisarmCommandQueued() ? "true" : "false")
           << ", telemetry_armed="
           << (telemetry_armed_ ? "true" : "false");

    send_zmq_action = true;
}

bool DemoH4HVehicle::isArmCommandQueued() const
{
    return pending_arm_command_ ==
           ArmCommand::kArm;
}

bool DemoH4HVehicle::isDisarmCommandQueued() const
{
    return pending_arm_command_ ==
           ArmCommand::kDisarm;
}

bool DemoH4HVehicle::isAutonomyEnabled() const
{
    if (vehicle_state_ == VehicleState::kAutonomy)
    {
        return false;
    }

    if (type_ == "UAV")
    {
        return telemetry_armed_;
    }

    return true;
}

bool DemoH4HVehicle::isAutonomyActive() const
{
    return vehicle_state_ ==
           VehicleState::kAutonomy;
}

//void DemoH4HVehicle::startAutonomy()
//{
//    if (!isAutonomyEnabled())
//    {
//        return;
//    }
//
//    vehicle_state_ = VehicleState::kAutonomy;
//    sender = "start_autonomy";
//    send_zmq_action = true;
//}

void DemoH4HVehicle::startAutonomy()
{
    if (!isAutonomyEnabled())
    {
        kLog.i << "Ignoring autonomy start request because autonomy is not enabled";
        return;
    }

    vehicle_state_ = VehicleState::kAutonomy;
    sender = "start_autonomy";

    kLog.i << "ACTION SEND TRIGGER: startAutonomy"
           << ", sender=" << sender
           << ", telemetry_armed="
           << (telemetry_armed_ ? "true" : "false");

    send_zmq_action = true;
}

//void DemoH4HVehicle::stopAutonomy()
//{
//    if (vehicle_state_ != VehicleState::kAutonomy)
//    {
//        return;
//    }
//
//    if (type_ == "UAV")
//    {
//        vehicle_state_ =
//                telemetry_armed_
//                ? VehicleState::kFlying
//                : VehicleState::kLanded;
//    }
//    else
//    {
//        vehicle_state_ = VehicleState::kTeleop;
//    }
//
//    sender = "stop_autonomy";
//    send_zmq_action = true;
//}

void DemoH4HVehicle::stopAutonomy()
{
    if (vehicle_state_ != VehicleState::kAutonomy)
    {
        kLog.i << "Ignoring autonomy stop request because autonomy is not active";
        return;
    }

    if (type_ == "UAV")
    {
        vehicle_state_ =
                telemetry_armed_
                ? VehicleState::kFlying
                : VehicleState::kLanded;
    }
    else
    {
        vehicle_state_ = VehicleState::kTeleop;
    }

    sender = "stop_autonomy";

    kLog.i << "ACTION SEND TRIGGER: stopAutonomy"
           << ", sender=" << sender
           << ", telemetry_armed="
           << (telemetry_armed_ ? "true" : "false");

    send_zmq_action = true;
}

bool DemoH4HVehicle::isArmEnabled() const
{
    /*
     * The Arm action may start only when confirmed telemetry says the UAV is
     * disarmed and no arm command is still waiting for local transmission.
     */
    return type_ == "UAV" &&
           !telemetry_armed_;
}


bool DemoH4HVehicle::isArmActive() const
{
    /*
     * Strict telemetry-driven status.
     *
     * Sending an arm request does not make this action active. The action only
     * becomes active after the UAV reports armed == true.
     */
    return type_ == "UAV" &&
           telemetry_armed_;
}

void DemoH4HVehicle::startArm()
{
    /*
     * Send an arm request without changing vehicle_state_.
     */
    queueArmCommand(ArmCommand::kArm);
}

void DemoH4HVehicle::stopArm()
{
    if (type_ != "UAV" || !telemetry_armed_)
    {
        return;
    }

    /*
     * Retain the existing warning/confirmation behavior, but only queue the
     * disarm command after confirmation. Do not set kLanded here.
     */
    auto notification =
            kinesis::Notification::build(
                    kinesis::Notification::Severity::kWarning,
                    name_,
                    "Disarm Called while armed.");

    notification->setDescription(
            "Confirm that the UAV should be disarmed.");

    notification->addButton(
            "Disarm UAV?",
            [this]()
            {
                queueArmCommand(ArmCommand::kDisarm);
            });

    notification->broadcast();
}

kinesis::PlatformInterface DemoH4HVehicle::getPlatform()
{
  kinesis::PlatformInterface platform;
  platform.name = name_;
  platform.make = kPlatformMake;
  platform.model = kPlatformModel;
  platform.update = [this]() { update(); };
  platform.get_location = [this]() { return getLocation(); };
  platform.get_energy_ratio = [this]() { return getEnergyRatio(); };
  platform.get_signal_ratio = [this]() { return getSignalRatio(); };
  platform.get_parameters = [this]() { return getParameters(); };
  platform.change_parameters = [this](auto p) { changeParameters(p); };
  platform.frame.get_is_enabled = []() { return true; };
  int axes;
  if (type_ == "UAV" || type_ == "Manipulator") {
    axes = kinesis::FrameInterface::kAxesLinear | kinesis::FrameInterface::kAxisFreeAngularZ;
  }
  else {
    axes = kinesis::FrameInterface::kAxisLinearX | kinesis::FrameInterface::kAxisFreeAngularZ;
  }

  platform.frame.get_controlled_axes = [axes]() { return axes; };
  platform.frame.get_angular_position = [this]() { return getAngularPosition(); };
//  platform.frame.get_linear_velocity_in_parent = [this]() { return getLinearVelocity(); };
//  auto linear_limits = kinesis::Vector(kVelocityLimitLinearXY, kVelocityLimitLinearXY, kVelocityLimitLinearZ);
//  //TODO: change to self
//  platform.frame.get_linear_velocity_limits_in_parent = [linear_limits]() { return linear_limits; };
//  auto angular_limits = kinesis::Vector(0.0, 0.0, kVelocityLimitAngularZ);
//  platform.frame.get_angular_velocity_limits_in_parent = [angular_limits]() { return angular_limits; };
//  platform.frame.set_linear_velocity_in_parent = [this](auto v) { return setLinearVelocity(v); };
//  platform.frame.set_angular_velocity_in_parent = [this](auto v) { return setAngularVelocity(v); };
  platform.frame.get_linear_velocity_in_self = [this]() { return linear_velocity_; };
  platform.frame.get_angular_velocity_in_self = [this]() { return angular_velocity_; };
  auto linear_limits = kinesis::Vector(kVelocityLimitLinearXY, kVelocityLimitLinearXY, kVelocityLimitLinearZ);
  platform.frame.get_linear_velocity_limits_in_self = [linear_limits]() { return linear_limits; };
  auto angular_limits = kinesis::Vector(0.0, 0.0, kVelocityLimitAngularZ);
  platform.frame.get_angular_velocity_limits_in_self = [angular_limits]() { return angular_limits; };
  platform.frame.set_linear_velocity_in_self = [this](auto v) { setLinearVelocity(v); };
  platform.frame.set_angular_velocity_in_self = [this](auto v) { setAngularVelocity(v); };
  return platform;
}

std::vector<kinesis::ActionInterface> DemoH4HVehicle::getActions()
{
  std::vector<kinesis::ActionInterface> action_interfaces;

  if (type_ == "UAV") {
      kinesis::ActionInterface arm;
      arm.name = "Arm";
      arm.is_start_enabled = [this]() { return isArmEnabled(); };
      arm.is_active = [this]() { return isArmActive(); };
      arm.start = [this]() { startArm(); };
      arm.stop = [this]() { stopArm(); };
      action_interfaces.push_back(arm);
  }

  kinesis::ActionInterface autonomy;
  autonomy.name = "Autonomy";
  autonomy.is_start_enabled = [this]() { return isAutonomyEnabled(); };
  autonomy.is_active = [this]() { return isAutonomyActive(); };
  autonomy.start = [this]() { startAutonomy(); };
  autonomy.stop = [this]() { stopAutonomy(); };
  action_interfaces.push_back(autonomy);

  return action_interfaces;
}
std::string DemoH4HVehicle::getUavMode(UavFlightMode mode) const {
  switch (mode) {
    case UavFlightMode::kHold:
      return "HOLD";
//    case UavFlightMode::kManual:
//      return "MANUAL";
    case UavFlightMode::kPosition:
      return "POSITION";
    case UavFlightMode::kOffboard:
      return "OFFBOARD";
  }
}

}  // namespace kinesis_example
