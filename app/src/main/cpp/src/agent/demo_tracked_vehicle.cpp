/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#include "agent/demo_tracked_vehicle.h"

#include "agent/demo_gimbal.h"
#include "agent/demo_view.h"

namespace kinesis_example {

bool DemoTrackedVehicle::constructNode(const std::string& name, const kinesis::Location& location)
{
  kinesis::NodeFactory factory(kIntegratorId);
  auto vehicle = std::make_shared<DemoTrackedVehicle>(name, location);
  factory.addPlatform(vehicle->getPlatformInterface());
  factory.addFrame(vehicle->gimbal_->getFrameInterface());
  for (const auto& view : vehicle->views_)
  {
    factory.addView(view->getViewInterface());
  }
  return factory.constructNode(vehicle) != kinesis::NodeFactory::kConstructionFailure;
}

DemoTrackedVehicle::DemoTrackedVehicle(const std::string& node_name, const kinesis::Location& initial_location)
  : name_(node_name)
  , location_(initial_location)
{
  gimbal_ =
      std::make_shared<DemoGimbal>("gimbal", kinesis::FrameInterface::kIdPlatform, kinesis::Vector(-0.05, 0.05, 0.15));

  // EO camera with 2x zoom pointing forward
  views_.push_back(std::make_shared<DemoView>("Front(EO)",
                                              "rtsp://streamserve.ok.ubc.ca:1935/timcam/timcam.stream",
                                              kinesis::ViewInterface::SensorType::kVisible,
                                              kinesis::ViewInterface::FieldOfView(90.0, 70.0),
                                              kinesis::FrameInterface::kIdPlatform,
                                              kinesis::Vector(),
                                              kinesis::Quaternion(),
                                              2.0,     // 2x zoom
                                              false,   // don't crop
                                              true));  // single image capture supported

  // EO camera with no zoom pointing backward
  views_.push_back(std::make_shared<DemoView>("Back(EO)",
                                              "rtsp://streamserve.ok.ubc.ca:1935/timcam/timcam.stream",
                                              kinesis::ViewInterface::SensorType::kVisible,
                                              kinesis::ViewInterface::FieldOfView(90.0, 70.0),
                                              kinesis::FrameInterface::kIdPlatform,
                                              kinesis::Vector(),
                                              kinesis::Quaternion::getRotationZ(M_PI)));

  // EO camera with 3x zoom on mast gimbal
  views_.push_back(std::make_shared<DemoView>("Mast(EO)",
                                              "rtsp://streamserve.ok.ubc.ca:1935/timcam/timcam.stream",
                                              kinesis::ViewInterface::SensorType::kVisible,
                                              kinesis::ViewInterface::FieldOfView(90.0, 70.0),
                                              gimbal_->getFrameInterface().id,
                                              kinesis::Vector(),
                                              kinesis::Quaternion(),
                                              3.0,     // 3x zoom
                                              false,   // don't crop
                                              true,    // single image capture supported
                                              0.0,     // periodic image capture not supported
                                              true));  // video capture supported));

  // IR camera with no zoom on mast gimbal
  views_.push_back(std::make_shared<DemoView>("Mast(IR)",
                                              "rtsp://stream.teracomm.bg:554/balkanika",
                                              kinesis::ViewInterface::SensorType::kInfrared,
                                              kinesis::ViewInterface::FieldOfView(90.0, 70.0),
                                              gimbal_->getFrameInterface().id));
}

DemoTrackedVehicle::~DemoTrackedVehicle()
{
}

kinesis::PlatformInterface DemoTrackedVehicle::getPlatformInterface()
{
  kinesis::PlatformInterface platform;
  platform.name = name_;
  platform.make = kPlatformMake;
  platform.model = kPlatformModel;
  platform.update = [this]() { update(); };
  platform.get_location = [this]() { return location_; };
  platform.get_energy_ratio = [this]() { return energy_ratio_; };
  platform.get_signal_ratio = [this]() { return signal_ratio_; };
  platform.get_parameters = [this]() { return getParameters(); };
  platform.change_parameters = [this](auto p) { changeParameters(p); };
  platform.frame.get_is_enabled = []() { return true; };
  auto axes = kinesis::FrameInterface::kAxisLinearX | kinesis::FrameInterface::kAxisFreeAngularZ;
  platform.frame.get_controlled_axes = [axes]() { return axes; };
  platform.frame.get_angular_position = [this]() { return attitude_; };
  platform.frame.get_linear_velocity_in_self = [this]() { return linear_velocity_; };
  platform.frame.get_angular_velocity_in_self = [this]() { return angular_velocity_; };
  auto linear_limits = kinesis::Vector(kVelocityLimitLinearX, 0.0, 0.0);
  platform.frame.get_linear_velocity_limits_in_self = [linear_limits]() { return linear_limits; };
  auto angular_limits = kinesis::Vector(0.0, 0.0, kVelocityLimitAngularZ);
  platform.frame.get_angular_velocity_limits_in_self = [angular_limits]() { return angular_limits; };
  platform.frame.set_linear_velocity_in_self = [this](auto v) { setLinearVelocity(v); };
  platform.frame.set_angular_velocity_in_self = [this](auto v) { setAngularVelocity(v); };
  return platform;
}

void DemoTrackedVehicle::initialize()
{
  kLog.v << "Initialize Called";
}

void DemoTrackedVehicle::update()
{
  long double seconds_since_last_update =
      std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - time_update_).count() /
      1000.0;
  time_update_ = std::chrono::steady_clock::now();

  // Simulate battery
  energy_ratio_ += -0.1 * seconds_since_last_update;
  if (energy_ratio_ < 0)
  {
    energy_ratio_ = 1.0;
  }

  // Simulate signal strength
  signal_ratio_ += -0.1 * seconds_since_last_update;
  if (signal_ratio_ < 0)
  {
    signal_ratio_ = 1.0;
  }

  // Simulate motion
  attitude_ = kinesis::Quaternion::getRotationZ(angular_velocity_.z * seconds_since_last_update) * attitude_;
  kinesis::Vector velocity_in_local = attitude_ * linear_velocity_;
  kinesis::Vector displacement = velocity_in_local * seconds_since_last_update;
  location_ = location_.getRelativeLocation(displacement.x, displacement.y, displacement.z);
}

void DemoTrackedVehicle::shutdown()
{
  kLog.v << "Shutdown Called";
}

kinesis::ParameterInterface DemoTrackedVehicle::getParameters() const
{
  std::string display_name = kParameterDisplayName;
  kinesis::ParameterInterface parameters;
  parameters.addParameterBool(display_name, toggle_parameter_);
  return parameters;
}

void DemoTrackedVehicle::changeParameters(const kinesis::ParameterInterface& changed_parameters)
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

void DemoTrackedVehicle::setLinearVelocity(kinesis::Vector velocity)
{
  //  kLog.v << std::fixed << std::setprecision(2) << "setLinearVelocity " << velocity;
  linear_velocity_.x = std::clamp(velocity.x, -kVelocityLimitLinearX, kVelocityLimitLinearX);
}

void DemoTrackedVehicle::setAngularVelocity(kinesis::Vector velocity)
{
  //  kLog.v << std::fixed << std::setprecision(2) << "setAngularVelocity " << velocity;
  angular_velocity_.z = std::clamp(velocity.z, -kVelocityLimitAngularZ, kVelocityLimitAngularZ);
}

}  // namespace kinesis_example
