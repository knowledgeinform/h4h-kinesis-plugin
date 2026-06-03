/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#include "agent/h4h_vehicle.h"
#include "zmq.hpp"
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
    energy_ratio_ = 0.5;
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
  long double seconds_since_last_update =
      std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - time_update_).count() /
      1000.0;
  time_update_ = std::chrono::steady_clock::now();

  if (feedback_) {
    zmq::message_t msg;
    auto res = status_socket.recv(msg, zmq::recv_flags::dontwait);
    if (res) {
      json from_uas = json::parse(msg.to_string());
      //      debug_to_uav.send(zmq::buffer(from_uas.dump()), zmq::send_flags::none);
      if (from_uas["messageType"] == "status") {
        location_.latitude = from_uas["messageData"]["lat"];
        location_.longitude = from_uas["messageData"]["lon"];
        location_.altitude = from_uas["messageData"]["alt"];
        energy_ratio_ = from_uas["messageData"]["powerLevel"];
      }
//      if (type_ == "UAV") {
//        flying_ = from_uas["messageData"]["flying"];
//        landed_ = from_uas["messageData"]["landed"];
//      }
    }
  }

  switch (vehicle_state_) {
    case VehicleState::kLanded:
      linear_velocity_ = kinesis::Vector(0.0, 0.0, 0.0);
      break;

    case VehicleState::kFlying:
      break;

    case VehicleState::kAutonomy:
      linear_velocity_ = kinesis::Vector(0.0, 0.0, 0.0);
      break;

    case VehicleState::kManual:
      break;

  }

  //Simulate Battery and Signal
//  energy_ratio_ = 1.0;
  signal_ratio_ = 1.0;

  // Simulate angular motion
//  if (vehicle_state_ != VehicleState::kFlying)
//  {
  yaw_ += angular_velocity_.z * seconds_since_last_update;
  if (yaw_ > M_PI)
  {
    yaw_ -= M_PI * 2.0;
  }
  else if (yaw_ < -M_PI)
  {
    yaw_ += M_PI * 2.0;
  }
//  }
  attitude_ = kinesis::Quaternion::getRotationZ(yaw_);

  // Simulate linear motion in the local-level frame
  kinesis::Vector displacement = linear_velocity_ * seconds_since_last_update;
  location_ = location_.getRelativeLocation(displacement.x, displacement.y, displacement.z);

  if (send_zmq){
            auto now = std::chrono::steady_clock::now();
            double time_since_last_message_ms = std::chrono::duration<double, std::milli>(
            now - last_lin_msg_time).count();
            if (time_since_last_message_ms > (1000.0 / zmqSendRateHz)) {
              json data = {
                {"name", name_},
                {"linear_x",    linear_velocity_.x},
                {"linear_y",    -linear_velocity_.y},
                {"linear_z",    linear_velocity_.z},
                {"angular_z",  -angular_velocity_.z}
              };

              if (type_ == "UAV") {
                  json uav = {
                          {"actions", {
                                  {"arm", (vehicle_state_ == VehicleState::kFlying)},
                                  {"disarm", (vehicle_state_ == VehicleState::kLanded)},
                                  {"autonomy", (vehicle_state_ == VehicleState::kAutonomy)}
                          }
                          }
                  };
                  data.update(uav);
              }
              else {
                json actions = {
                        {"actions", {
                                {"autonomy", (vehicle_state_ == VehicleState::kAutonomy)}
                        }
                        }
                };
                data.update(actions);
              }

                json msgOut = {
                      {"messageType", "control"},
                      {"messageData", data}
              };

//               send out the message
              mobility_socket.send(zmq::buffer(msgOut.dump()), zmq::send_flags::none);

              send_zmq = false;
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
  parameters.addParameterBool(display_name, toggle_parameter_);
  return parameters;
}

void DemoH4HVehicle::changeParameters(const kinesis::ParameterInterface& changed_parameters)
{
  kLog.v << "changeParameters() called with: " << changed_parameters.dumpAsString();
  // update the toggle_parameter_ value based on changed parameters call (if necessary)
  std::vector<std::string> display_names = changed_parameters.getDisplayNames();
  for (auto& name : display_names)
  {
    if (kParameterDisplayName == name)
    {
      toggle_parameter_ = changed_parameters.getParameterBool(kParameterDisplayName).value_or(toggle_parameter_);
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

  send_zmq = true;
  }

void DemoH4HVehicle::setAngularVelocity(kinesis::Vector velocity)
{
  //  kLog.v << std::fixed << std::setprecision(2) << "setAngularVelocity " << velocity;
  angular_velocity_.z = (std::clamp(velocity.z, -kVelocityLimitAngularZ, kVelocityLimitAngularZ))/kNormalizeVelAng;
  send_zmq = true;
  }


bool DemoH4HVehicle::isAutonomyEnabled() const
{
  return (vehicle_state_ != VehicleState::kAutonomy);
}

bool DemoH4HVehicle::isAutonomyActive() const
{
  return (vehicle_state_ == VehicleState::kAutonomy);
}

void DemoH4HVehicle::startAutonomy() {
  send_zmq = true;
  if (vehicle_state_ != VehicleState::kAutonomy) {
    vehicle_state_ = VehicleState::kAutonomy;
  }
}

void DemoH4HVehicle::stopAutonomy() {
  send_zmq = true;
  if (vehicle_state_ == VehicleState::kAutonomy) {
    if (type_ == "UAV") {
      vehicle_state_ = VehicleState::kFlying;
    }
    else {
      vehicle_state_ = VehicleState::kManual;
    }

  }
}

bool DemoH4HVehicle::isArmEnabled() const
{
  return (vehicle_state_ == VehicleState::kLanded);
}

bool DemoH4HVehicle::isArmActive() const
{
  return (vehicle_state_ == VehicleState::kFlying || vehicle_state_ == VehicleState::kAutonomy);
}

void DemoH4HVehicle::startArm() {
  send_zmq = true;
  if (vehicle_state_ != VehicleState::kFlying && vehicle_state_ != VehicleState::kAutonomy) {
    vehicle_state_ = VehicleState::kFlying;
  }
}

void DemoH4HVehicle::stopArm() {
  send_zmq = true;
  // TODOD: add safety thingsss
  if (vehicle_state_ == VehicleState::kFlying) {
    vehicle_state_ = VehicleState::kLanded;
  }
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
  if (type_ == "UAV") {
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

}  // namespace kinesis_example
