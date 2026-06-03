/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#include "agent/demo_hovering_vehicle.h"

#include "agent/demo_gimbal.h"
#include "agent/demo_view.h"

#include <cmath>
#include <iomanip>

namespace kinesis_example {

bool DemoHoveringVehicle::constructNode(const std::string& name, const kinesis::Location& location)
{
  kinesis::NodeFactory factory(kIntegratorId);
  auto vehicle = std::make_shared<DemoHoveringVehicle>(name, location);
  factory.addPlatform(vehicle->getPlatform());
  factory.addActions(vehicle->getActions());
  factory.addFrame(vehicle->gimbal_->getFrameInterface());
  for (const auto& view : vehicle->views_)
  {
    factory.addView(view->getViewInterface());
  }
  return factory.constructNode(vehicle) != kinesis::NodeFactory::kConstructionFailure;
}

DemoHoveringVehicle::DemoHoveringVehicle(const std::string& node_name, const kinesis::Location& initial_location)
  : name_(node_name)
  , location_(initial_location)
{
  location_.altitude_reference = kinesis::Location::AltitudeReference::kTakeoff;

  gimbal_ =
      std::make_shared<DemoGimbal>("gimbal", kinesis::FrameInterface::kIdPlatform, kinesis::Vector(0.05, 0.0, 0.0));

  // Downward EO camera with no zoom
  views_.push_back(std::make_shared<DemoView>("Down(EO)",
                                              "rtsp://streamserve.ok.ubc.ca:1935/timcam/timcam.stream",
                                              kinesis::ViewInterface::SensorType::kVisible,
                                              kinesis::ViewInterface::FieldOfView(90.0, 70.0),
                                              kinesis::FrameInterface::kIdPlatform,       // parent ID
                                              kinesis::Vector(),                          // linear offset
                                              kinesis::Quaternion::getRotationY(M_PI_2),  // angular offset
                                              0.0,                                        // no zoom
                                              false,                                      // don't crop
                                              true,   // single image capture supported
                                              5.0));  // periodic image capture supported (5 sec interval)

  // Gimbaled EO camera with 5x zoom
  views_.push_back(std::make_shared<DemoView>("Gimbal(EO)",
                                              "rtsp://streamserve.ok.ubc.ca:1935/timcam/timcam.stream",
                                              kinesis::ViewInterface::SensorType::kVisible,
                                              kinesis::ViewInterface::FieldOfView(90.0, 70.0),
                                              gimbal_->getFrameInterface().id,
                                              kinesis::Vector(),
                                              kinesis::Quaternion(),
                                              5.0,     // 5x zoom
                                              false,   // don't crop
                                              true,    // single image capture supported
                                              5.0,     // periodic image capture supported (5 sec interval)
                                              true));  // video capture supported

  // Gimbaled IR camera with no zoom
  views_.push_back(std::make_shared<DemoView>("Gimbal(IR)",
                                              "rtsp://stream.teracomm.bg:554/balkanika",
                                              kinesis::ViewInterface::SensorType::kInfrared,
                                              kinesis::ViewInterface::FieldOfView(80.0, 60.0),
                                              gimbal_->getFrameInterface().id,
                                              kinesis::Vector(),
                                              kinesis::Quaternion(),
                                              0.0,     // no zoom
                                              false,   // don't crop
                                              true));  // single image capture supported
}

DemoHoveringVehicle::~DemoHoveringVehicle()
{
}

void DemoHoveringVehicle::initialize()
{
  kLog.v << "Initialize Called";
}

void DemoHoveringVehicle::update()
{
  long double seconds_since_last_update =
      std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - time_update_).count() /
      1000.0;
  time_update_ = std::chrono::steady_clock::now();

  switch (vehicle_state_)
  {
    case VehicleState::kLanded:
      linear_velocity_ = kinesis::Vector(0.0, 0.0, 0.0);
      break;

    case VehicleState::kTakingOff:
      if (location_.altitude > takeoff_altitude_)  // if finished taking off...
      {
        vehicle_state_ = VehicleState::kFlying;
        linear_velocity_ = kinesis::Vector(0.0, 0.0, 0.0);
        kLog.d << "Takeoff complete";
      }
      else  // if still taking off...
      {
        linear_velocity_ = kinesis::Vector(0.0, 0.0, takeoff_landing_speed_);
      }
      break;

    case VehicleState::kFlying:
      break;

    case VehicleState::kLanding:
      if (location_.altitude <= 0.0)  // if finished landing...
      {
        vehicle_state_ = VehicleState::kLanded;
        linear_velocity_ = kinesis::Vector(0.0, 0.0, 0.0);
        location_.altitude = 0.0;
        kLog.d << "Landing complete";
      }
      else  // if still landing...
      {
        linear_velocity_ = kinesis::Vector(0.0, 0.0, -takeoff_landing_speed_);
      }
      break;
  }

  // Update battery
  // Battery and signal ratio cycle through 1.0-0.0 over 10 seconds to test the display of battery and signal values
  energy_ratio_ += -0.1 * seconds_since_last_update;
  if (energy_ratio_ < 0)
  {
    energy_ratio_ = 1.0;
  }
  // Update the Signal Ratio
  signal_ratio_ += -0.1 * seconds_since_last_update;
  if (signal_ratio_ < 0)
  {
    signal_ratio_ = 1.0;
  }

  // Simulate angular motion
  if (vehicle_state_ == VehicleState::kFlying)
  {
    yaw_ += angular_velocity_.z * seconds_since_last_update;
    if (yaw_ > M_PI)
    {
      yaw_ -= M_PI * 2.0;
    }
    else if (yaw_ < -M_PI)
    {
      yaw_ += M_PI * 2.0;
    }
  }
  attitude_ = kinesis::Quaternion::getRotationZ(yaw_);

  // Simulate linear motion in the local-level frame
  kinesis::Vector displacement = linear_velocity_ * seconds_since_last_update;
  location_ = location_.getRelativeLocation(displacement.x, displacement.y, displacement.z);
}

void DemoHoveringVehicle::shutdown()
{
  kLog.v << "Shutdown Called";
}

kinesis::Location DemoHoveringVehicle::getLocation() const
{
  return location_;
}

double DemoHoveringVehicle::getEnergyRatio() const
{
  return energy_ratio_;
}

double DemoHoveringVehicle::getSignalRatio() const
{
  return signal_ratio_;
}

kinesis::ParameterInterface DemoHoveringVehicle::getParameters() const
{
  std::string display_name = kParameterDisplayName;
  kinesis::ParameterInterface parameters;
  parameters.addParameterBool(display_name, toggle_parameter_);
  return parameters;
}

void DemoHoveringVehicle::changeParameters(const kinesis::ParameterInterface& changed_parameters)
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

bool DemoHoveringVehicle::isControlEnabled() const
{
  return (vehicle_state_ == VehicleState::kFlying);
}

kinesis::Quaternion DemoHoveringVehicle::getAngularPosition()
{
  return attitude_;
}

kinesis::Vector DemoHoveringVehicle::getLinearVelocity() const
{
  return linear_velocity_;
}

void DemoHoveringVehicle::setLinearVelocity(kinesis::Vector velocity)
{
  //  kLog.v << std::fixed << std::setprecision(2) << "setLinearVelocity " << velocity;
  linear_velocity_.x = std::clamp(velocity.x, -kVelocityLimitLinearXY, kVelocityLimitLinearXY);
  linear_velocity_.y = std::clamp(velocity.y, -kVelocityLimitLinearXY, kVelocityLimitLinearXY);
  linear_velocity_.z = std::clamp(velocity.z, -kVelocityLimitLinearZ, kVelocityLimitLinearZ);
}

void DemoHoveringVehicle::setAngularVelocity(kinesis::Vector velocity)
{
  //  kLog.v << std::fixed << std::setprecision(2) << "setAngularVelocity " << velocity;
  angular_velocity_.z = std::clamp(velocity.z, -kVelocityLimitAngularZ, kVelocityLimitAngularZ);
}

bool DemoHoveringVehicle::isTakeoffEnabled() const
{
  return (vehicle_state_ == VehicleState::kLanded);
}

bool DemoHoveringVehicle::isTakeoffActive() const
{
  return (vehicle_state_ == VehicleState::kTakingOff);
}

void DemoHoveringVehicle::startTakeoff()
{
  if (vehicle_state_ == VehicleState::kLanded)
  {
    kLog.d << "Takeoff started";
    vehicle_state_ = VehicleState::kTakingOff;
  }
}

void DemoHoveringVehicle::stopTakeoff()
{
  if (vehicle_state_ == VehicleState::kTakingOff)
  {
    kLog.d << "Takeoff stopped";
    vehicle_state_ = VehicleState::kFlying;
    linear_velocity_ = kinesis::Vector(0.0, 0.0, 0.0);
  }
}

bool DemoHoveringVehicle::isLandEnabled() const
{
  return (vehicle_state_ == VehicleState::kFlying);
}

bool DemoHoveringVehicle::isLandActive() const
{
  return (vehicle_state_ == VehicleState::kLanding);
}

void DemoHoveringVehicle::startLand()
{
  if (vehicle_state_ == VehicleState::kFlying)
  {
    kLog.d << "Landing started";
    vehicle_state_ = VehicleState::kLanding;
  }
}

void DemoHoveringVehicle::stopLand()
{
  if (vehicle_state_ == VehicleState::kLanding)
  {
    kLog.d << "Landing stopped";
    vehicle_state_ = VehicleState::kFlying;
    linear_velocity_ = kinesis::Vector(0.0, 0.0, 0.0);
  }
}

kinesis::PlatformInterface DemoHoveringVehicle::getPlatform()
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
  platform.frame.get_is_enabled = [this]() { return isControlEnabled(); };
  auto axes = kinesis::FrameInterface::kAxesLinear | kinesis::FrameInterface::kAxisFreeAngularZ;
  platform.frame.get_controlled_axes = [axes]() { return axes; };
  platform.frame.get_angular_position = [this]() { return getAngularPosition(); };
  platform.frame.get_linear_velocity_in_parent = [this]() { return getLinearVelocity(); };
  auto linear_limits = kinesis::Vector(kVelocityLimitLinearXY, kVelocityLimitLinearXY, kVelocityLimitLinearZ);
  platform.frame.get_linear_velocity_limits_in_parent = [linear_limits]() { return linear_limits; };
  auto angular_limits = kinesis::Vector(0.0, 0.0, kVelocityLimitAngularZ);
  platform.frame.get_angular_velocity_limits_in_parent = [angular_limits]() { return angular_limits; };
  platform.frame.set_linear_velocity_in_parent = [this](auto v) { return setLinearVelocity(v); };
  platform.frame.set_angular_velocity_in_parent = [this](auto v) { return setAngularVelocity(v); };
  return platform;
}

std::vector<kinesis::ActionInterface> DemoHoveringVehicle::getActions()
{
  std::vector<kinesis::ActionInterface> action_interfaces;

  kinesis::ActionInterface takeoff;
  takeoff.name = "Takeoff";
  takeoff.is_start_enabled = [this]() { return isTakeoffEnabled(); };
  takeoff.is_active = [this]() { return isTakeoffActive(); };
  takeoff.start = [this]() { startTakeoff(); };
  takeoff.stop = [this]() { stopTakeoff(); };
  action_interfaces.push_back(takeoff);

  kinesis::ActionInterface land;
  land.name = "Land";
  land.is_start_enabled = [this]() { return isLandEnabled(); };
  land.is_active = [this]() { return isLandActive(); };
  land.start = [this]() { startLand(); };
  land.stop = [this]() { stopLand(); };
  action_interfaces.push_back(land);

  return action_interfaces;
}

}  // namespace kinesis_example
