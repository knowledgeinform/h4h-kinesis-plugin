/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#include "agent/demo_fixed_wing_vehicle.h"

#include "agent/demo_gimbal.h"
#include "agent/demo_location.h"
#include "agent/demo_view.h"

#include <iomanip>

namespace kinesis_example {

bool DemoFixedWingVehicle::constructNode(const std::string& name, const kinesis::Location& location)
{
  kinesis::NodeFactory factory(kIntegratorId);
  auto vehicle = std::make_shared<DemoFixedWingVehicle>(name, location);
  factory.addPlatform(vehicle->getPlatformInterface());
  factory.addFrame(vehicle->gimbal_->getFrameInterface());
  for (const auto& view : vehicle->views_)
  {
    factory.addView(view->getViewInterface());
  }
  factory.addLocation(vehicle->location_approach_->getLocationInterface());
  factory.addLocation(vehicle->location_land_->getLocationInterface());
  factory.addLocation(vehicle->location_loiter_->getLocationInterface());
  factory.addLocation(vehicle->location_view_->getLocationInterface());
  return factory.constructNode(vehicle) != kinesis::NodeFactory::kConstructionFailure;
}

DemoFixedWingVehicle::DemoFixedWingVehicle(const std::string& platform_name, const kinesis::Location& initial_location)
  : name_(platform_name)
  , vehicle_location_(initial_location)
{
  gimbal_ =
      std::make_shared<DemoGimbal>("gimbal", kinesis::FrameInterface::kIdPlatform, kinesis::Vector(0.05, 0.0, 0.0));

  // EO camera on the platform looking left
  views_.push_back(std::make_shared<DemoView>("Left(EO)",
                                              "rtsp://streamserve.ok.ubc.ca:1935/timcam/timcam.stream",
                                              kinesis::ViewInterface::SensorType::kVisible,
                                              kinesis::ViewInterface::FieldOfView(90.0, 70.0),
                                              kinesis::FrameInterface::kIdPlatform,
                                              kinesis::Vector(0.1, 0.05, 0),
                                              kinesis::Quaternion::getRotationZ(M_PI_2),
                                              0.0,     // no zoom
                                              false,   // don't crop
                                              true));  // single image capture supported

  // EO camera on the gimbal
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
                                              0.0,     // periodic image capture not supported
                                              true));  // video capture supported

  // IR camera with no zoom on the gimbal
  views_.push_back(std::make_shared<DemoView>("Gimbal(IR)",
                                              "rtsp://stream.teracomm.bg:554/balkanika",
                                              kinesis::ViewInterface::SensorType::kInfrared,
                                              kinesis::ViewInterface::FieldOfView(80.0, 60.0),
                                              gimbal_->getFrameInterface().id));

  // Landing location (example of a home location that can be moved)
  location_land_ = std::make_shared<DemoLocation>("Landing",
                                                  "Location where vehicle will land",
                                                  kinesis::Location::Type::kHome,
                                                  initial_location,
                                                  true);

  // Landing approach location (example of a POI location that can be moved)
  location_approach_ = std::make_shared<DemoLocation>("Approach",
                                                      "Location from which vehicle will approach when landing",
                                                      kinesis::Location::Type::kGeneric,
                                                      initial_location.getRelativeLocation(50, 50, 0),
                                                      true);

  // Loiter location (example of a POI location that can be moved)
  location_loiter_ = std::make_shared<DemoLocation>("Loiter",
                                                    "Location where vehicle will loiter",
                                                    kinesis::Location::Type::kGeneric,
                                                    initial_location.getRelativeLocation(5, 0, 0),
                                                    true);

  // View center location (example of a view center location that cannot be moved)
  location_view_ = std::make_shared<DemoLocation>("View",
                                                  "Location where view line-of-sight intersects ground",
                                                  kinesis::Location::Type::kViewCenter,
                                                  kinesis::Location());
}

DemoFixedWingVehicle::~DemoFixedWingVehicle()
{
}

kinesis::PlatformInterface DemoFixedWingVehicle::getPlatformInterface()
{
  kinesis::PlatformInterface platform;
  platform.name = name_;
  platform.make = kPlatformMake;
  platform.model = kPlatformModel;
  platform.update = [this]() { update(); };
  platform.get_location = [this]() { return getLocation(); };
  platform.get_energy_ratio = [this]() { return getEnergyRatio(); };
  platform.get_signal_ratio = [this]() { return getSignalRatio(); };
  platform.frame.get_is_enabled = []() { return true; };
  auto axes = kinesis::FrameInterface::kAxisLinearX | kinesis::FrameInterface::kAxisSteeredAngularY |
              kinesis::FrameInterface::kAxisSteeredAngularZ;
  platform.frame.get_controlled_axes = [axes]() { return axes; };
  platform.frame.get_angular_position = [this]() { return getAngularPosition(); };
  platform.frame.get_linear_velocity_in_self = [this]() { return getLinearVelocity(); };
  auto linear_limits = kinesis::Vector(kVelocityLimitLinearX, 0.0, 0.0);
  platform.frame.get_linear_velocity_limits_in_self = [linear_limits]() { return linear_limits; };
  auto angular_limits = kinesis::Vector(0.0, kVelocityLimitAngularY, kVelocityLimitAngularZ);
  platform.frame.get_angular_velocity_limits_in_self = [angular_limits]() { return angular_limits; };
  platform.frame.set_linear_velocity_in_self = [this](auto v) { setLinearVelocity(v); };
  platform.frame.set_angular_velocity_in_self = [this](auto v) { setAngularVelocity(v); };
  return platform;
}

void DemoFixedWingVehicle::initialize()
{
  kLog.v << "Initialize Called";
}

void DemoFixedWingVehicle::update()
{
  long double seconds_since_last_update =
      std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - time_update_).count() /
      1000.0;
  time_update_ = std::chrono::steady_clock::now();

  // Update energy ratio (simulated here)
  if (energy_ratio_ > 0.0)
  {
    energy_ratio_ -= kBatteryDepletionRate * seconds_since_last_update;
  }
  else
  {
    energy_ratio_ = 0.0;
  }

  // Update angular position (simulated here)
  double pitch_change = (angular_velocity_.y - kPitchLevelingGain * pitch_) * seconds_since_last_update;
  pitch_ = std::clamp(pitch_ + pitch_change, -kPitchLimit, kPitchLimit);
  yaw_ += angular_velocity_.z * seconds_since_last_update;
  if (yaw_ > M_PI)
  {
    yaw_ -= 2.0 * M_PI;
  }
  else if (yaw_ < M_PI)
  {
    yaw_ += 2.0 * M_PI;
  }
  attitude_ = kinesis::Quaternion::getRotationZ(yaw_) * kinesis::Quaternion::getRotationY(pitch_);

  // Update location (simulated here)
  kinesis::Vector velocity_in_local = attitude_ * linear_velocity_;
  double stall_ratio = std::clamp(1.0 - linear_velocity_.x / (kStallSpeedRatio * kVelocityLimitLinearX), 0.0, 1.0);
  double stall_velocity = kVelocityLimitLinearX * stall_ratio * stall_ratio;
  velocity_in_local.z = velocity_in_local.z - stall_velocity;
  kinesis::Vector displacement = velocity_in_local * seconds_since_last_update;
  vehicle_location_ = vehicle_location_.getRelativeLocation(displacement.x, displacement.y, displacement.z);

  // Simulate landing/crashing
  if (vehicle_location_.altitude < ground_altitude_)  // if just landed/crashed...
  {
    vehicle_location_.altitude = ground_altitude_;
    pitch_ = 0.0;
    linear_velocity_ = kinesis::Vector();
    angular_velocity_ = kinesis::Vector();
  }

  // Update view center location (simulated here with a flat ground assumption)
  kinesis::Location view_center;  // default constructor is an invalid location
  auto view_vector = attitude_ * gimbal_->getAngularPosition() * kinesis::Vector(1, 0, 0);
  if (view_vector.z < 0.0)  // if looking down...
  {
    view_vector = view_vector * ((vehicle_location_.altitude - ground_altitude_) / -view_vector.z);
    view_center = vehicle_location_.getRelativeLocation(view_vector.x, view_vector.y, view_vector.z);
  }
  location_view_->updateLocation(view_center);

  is_flying_ = (vehicle_location_.altitude > ground_altitude_);
}

void DemoFixedWingVehicle::shutdown()
{
  kLog.v << "Shutdown Called";
}

kinesis::Location DemoFixedWingVehicle::getLocation() const
{
  return vehicle_location_;
}

double DemoFixedWingVehicle::getEnergyRatio() const
{
  return energy_ratio_;
}

double DemoFixedWingVehicle::getSignalRatio() const
{
  return signal_ratio_;
}

kinesis::Quaternion DemoFixedWingVehicle::getAngularPosition()
{
  return attitude_;
}

kinesis::Vector DemoFixedWingVehicle::getLinearVelocity() const
{
  return linear_velocity_;
}

void DemoFixedWingVehicle::setLinearVelocity(kinesis::Vector velocity)
{
  if (is_flying_ && (velocity.x == 0.0))
  {
    // TODO: decide what to do if commanded to stop while flying
    // (possibly due to a disconnection from the user)
  }
  else
  {
    linear_velocity_.x = std::clamp(velocity.x, 0.0, kVelocityLimitLinearX);
    //  kLog.v << std::fixed << std::setprecision(2) << "setLinearVelocity " << velocity;
  }
}

void DemoFixedWingVehicle::setAngularVelocity(kinesis::Vector velocity)
{
  double speed_ratio = linear_velocity_.x / kVelocityLimitLinearX;
  double pitch_ratio = std::clamp(velocity.y / kVelocityLimitAngularY, -1.0, 1.0);
  double yaw_ratio = std::clamp(velocity.z / kVelocityLimitAngularZ, -1.0, 1.0);
  angular_velocity_.y = speed_ratio * pitch_ratio * kVelocityLimitAngularY;
  angular_velocity_.z = speed_ratio * yaw_ratio * kVelocityLimitAngularZ;
  //  kLog.v << std::fixed << std::setprecision(2) << "setAngularVelocity: speed: " << speed_ratio
  //         << ", pitch: " << pitch_ratio << " (" << angular_velocity_.y << " rad/s), yaw: " << yaw_ratio << " ("
  //         << angular_velocity_.z << ")";
}

}  // namespace kinesis_example
