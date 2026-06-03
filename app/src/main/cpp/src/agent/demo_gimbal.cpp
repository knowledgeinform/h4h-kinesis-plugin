/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#include "agent/demo_gimbal.h"

#include <iomanip>

namespace kinesis_example {

DemoGimbal::DemoGimbal(const std::string& id,
                       const std::string& parent_id,
                       const kinesis::Vector& linear_position,
                       const kinesis::Quaternion& initial_angular_position)
  : angular_position_(initial_angular_position)
{
  frame_interface_.id = id;
  frame_interface_.parent_id = parent_id;
  frame_interface_.get_is_enabled = []() { return true; };
  auto axes = kinesis::FrameInterface::kAxisFreeAngularY | kinesis::FrameInterface::kAxisFreeAngularZ;
  frame_interface_.get_controlled_axes = [axes]() { return axes; };
  frame_interface_.get_linear_position = [linear_position]() { return linear_position; };
  frame_interface_.get_angular_position = [this]() { return getAngularPosition(); };
  auto limits = kinesis::Vector(0.0, kVelocityLimitAngular, kVelocityLimitAngular);
  frame_interface_.get_angular_velocity_limits_in_self = [limits]() { return limits; };
  frame_interface_.set_angular_velocity_in_self = [this](auto v) { setAngularVelocity(v); };
  frame_interface_.set_angular_position = [this](auto p) { setAngularPosition(p); };
}

DemoGimbal::~DemoGimbal()
{
}

kinesis::FrameInterface DemoGimbal::getFrameInterface()
{
  return frame_interface_;
}

kinesis::Quaternion DemoGimbal::getAngularPosition()
{
  auto now = std::chrono::steady_clock::now();
  long double seconds_since_last_call =
      std::chrono::duration_cast<std::chrono::milliseconds>(now - time_position_checked_).count() / 1000.0;
  time_position_checked_ = now;

  if (seconds_since_last_call < kTimeoutVelocity)
  {
    auto displacement = angular_velocity_ * seconds_since_last_call;
    angular_position_ = angular_position_ * kinesis::Quaternion(displacement);
  }
  else  // if timed out...
  {
    angular_velocity_.x = 0.0;
    angular_velocity_.y = 0.0;
    angular_velocity_.z = 0.0;
  }
  return angular_position_;
}

void DemoGimbal::setAngularVelocity(kinesis::Vector angular_velocity)
{
  kLog.v << "Setting angular velocity of gimbal '" << frame_interface_.id << "' to " << std::fixed
         << std::setprecision(2) << angular_velocity;
  time_position_checked_ = std::chrono::steady_clock::now();  // reset velocity timeout
  angular_velocity_ = angular_velocity;
}

void DemoGimbal::setAngularPosition(kinesis::Quaternion quaternion)
{
  kLog.v << "Setting angular position of gimbal '" << frame_interface_.id << "' to " << quaternion;
  angular_velocity_.x = 0.0;
  angular_velocity_.y = 0.0;
  angular_velocity_.z = 0.0;
  angular_position_ = quaternion;  // simulate a really fast gimbal
}

}  // namespace kinesis_example
