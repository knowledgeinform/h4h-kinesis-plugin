/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include "sdk/frame_interface.h"
#include "sdk/logger.h"

#include <chrono>

namespace kinesis_example {

/** @class DemoGimbal
 * A simulated two-axis gimbal that can rotate about its y and z axes with respect to a parent coordinate frame
 */
class DemoGimbal
{
public:
  /**
   * Constructor
   * @param id - ID of this gimbal
   * @param parent_id - ID of the parent of this gimbal
   * @param linear_position - linear position within the parent frame (m)
   * @param initial_angular_position - initial angular position with respect to the parent frame (default: identity)
   */
  DemoGimbal(const std::string& id,
             const std::string& parent_id,
             const kinesis::Vector& linear_position,
             const kinesis::Quaternion& initial_angular_position = kinesis::Quaternion());

  /**
   * Destructor
   */
  virtual ~DemoGimbal();

  /**
   * Get the frame interface
   * @return FrameInterface for Node Factory
   */
  kinesis::FrameInterface getFrameInterface();

  /**
   * Get angular position in parent frame
   * @return quaternion that rotates from this frame to parent frame
   */
  kinesis::Quaternion getAngularPosition();

  /**
   * Set angular velocity in this frame
   * @param velocity - angular velocity in this frame (rad/s)
   */
  void setAngularVelocity(kinesis::Vector velocity);

  /**
   * Set angular position in the parent frame
   * @param rotation_to_parent - quaternion that rotates from this frame to its parent frame
   */
  void setAngularPosition(kinesis::Quaternion rotation_to_parent);

private:
  static constexpr kinesis::Logger kLog = kinesis::Logger("DemoGimbal");

  /**
   * Angular velocity limit
   */
  static constexpr double kVelocityLimitAngular = 1.0;

  /**
   * Timeout for velocity command
   */
  static constexpr double kTimeoutVelocity = 0.5;

  /**
   * Frame interface
   */
  kinesis::FrameInterface frame_interface_;

  /**
   * Angular position (quaternion rotation to the parent coordinate frame)
   */
  kinesis::Quaternion angular_position_;

  /**
   * Angular velocity (expressed in end frame)
   */
  kinesis::Vector angular_velocity_;

  /**
   * Last time that the position was checked
   */
  std::chrono::steady_clock::time_point time_position_checked_;
};

}  // namespace kinesis_example
