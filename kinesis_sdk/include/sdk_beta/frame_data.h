/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include "sdk/controlled_axes.h"
#include "sdk/quaternion.h"

#include <optional>
#include <string>

namespace kinesis {
struct FrameData
{
  /// The node this frame belongs to
  std::string node_id;

  /// The ID of the frame
  std::string frame_id;

  /**
   * @brief The ID of the parent frame
   *
   * The parent frame is the coordinate system that the current frame is defined relative to. For example, the gimbal
   * frame of a vehicle is typically defined relative to its parent frame, which is often the platform frame.
   *
   * In Kinesis, all platform frames are defined relative to the local-level frame, whose ID is defined as
   * @ref FrameInterface::kIdLocalLevel.
   */
  std::string parent_id;

  /**
   * @brief Struct used to determine which axes of the given frame can be controlled
   *
   * This struct is populated based on the value returned from @ref FrameInterface::get_controlled_axes.
   *
   * For example, the `linear_x` flag in this struct can be used to determine whether the frame allows movement along
   * the linear X axis.
   */
  ControlledAxes controllable_axes;

  /**
   * @brief Angular velocity limits of this frame expressed in this frame
   *
   * The maximum magnitudes (radians/second) of each component of a desired velocity vector that will be accepted by
   * @ref FrameClient::setAngularVelocityInSelf()
   */
  std::optional<Vector> angular_velocity_limits_in_self;

  /**
   * @brief Angular velocity limits of this frame expressed in its parent frame
   *
   * The maximum magnitudes (radians/second) of each component of a desired velocity vector that will be accepted by
   * @ref FrameClient::setAngularVelocityInParent()
   */
  std::optional<Vector> angular_velocity_limits_in_parent;

  /**
   * @brief Linear velocity limits of this frame expressed in this frame
   *
   * The maximum magnitudes (meters/second) of each component of a desired velocity vector.
   */
  std::optional<Vector> linear_velocity_limits_in_self;

  /**
   * @brief Linear velocity limits of this frame expressed in its parent frame
   *
   * The maximum magnitudes (meters/second) of each component of a desired velocity vector.
   */
  std::optional<Vector> linear_velocity_limits_in_parent;

  /**
   * @brief Linear velocity of this frame expressed in this frame
   *
   * The current linear velocity vector (in meters/second) of the frame.
   */
  std::optional<Vector> linear_velocity_in_self;

  /**
   * @brief Linear velocity of this frame expressed in its parent frame
   *
   * The current linear velocity vector (in meters/second) of the parent frame.
   */
  std::optional<Vector> linear_velocity_in_parent;

  /// Reported linear position of the given frame
  Vector linear_position;

  /**
   * @brief Angular velocity of this frame expressed in this frame
   *
   * The current angular velocity vector (in radian/second) of the frame.
   */
  std::optional<Vector> angular_velocity_in_self;

  /**
   * @brief Angular velocity of this frame expressed in its parent frame
   *
   * The current angular velocity vector (in radians/second) of the parent frame.
   */
  std::optional<Vector> angular_velocity_in_parent;

  /// Reported angular position of the given frame
  Quaternion angular_position;

  /// Flag indicating if this frame can currently set linear velocity ratio
  bool is_set_linear_velocity_ratio_enabled;

  /// Flag indicating if this frame can currently set angular velocity
  bool is_set_angular_velocity_enabled;

  /// Flag indicating if this frame can currently set angular velocity ratio
  bool is_set_angular_velocity_ratio_enabled;

  /// Flag indicating if this frame can currently set angular displacement
  bool is_set_angular_displacement_enabled;

  /// Flag indicating if this frame can currently set linear acceleration
  bool is_set_linear_acceleration_enabled;

  /// Flag indicating if this frame can currently set angular position
  bool is_set_angular_position_enabled;

  bool operator==(const FrameData& other) const;
  bool operator!=(const FrameData& other) const;
};

}  // namespace kinesis
