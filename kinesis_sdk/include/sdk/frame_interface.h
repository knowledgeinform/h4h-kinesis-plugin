/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include "sdk/quaternion.h"
#include "sdk/vector.h"

#include <functional>
#include <string>

namespace kinesis {

/**
 * @brief Interface for a coordinate frame.
 * Where applicable, the axes of this frame are defined as:
 *   X - Forward
 *   Y - Left
 *   Z - Up
 */
struct FrameInterface
{
  /// ID for local-level frame
  static constexpr char kIdLocalLevel[] = "local_level";

  /// ID for platform frame
  static constexpr char kIdPlatform[] = "platform";

  /**
   * Optional field for the ID of this frame.
   * Must be unique withing this platform.
   */
  std::string id;

  /**
   * Required field for the ID of the parent of this frame.
   * Valid options include:
   * - FrameInterface::kIdLocalLevel
   * - FrameInterface::kIdPlatform
   * - id field value used in another FrameInterface
   */
  std::string parent_id;

  /**
   * Method to get whether this interface is enabled for control.
   * Required if frame can be controlled.
   * @return true if ready to respond to commands
   */
  std::function<bool()> get_is_enabled;

  /**
   * Method to get the axes (of this frame) of motion that can be controlled.
   * Required if frame can be controlled.
   * @return uint32_t - Bitwise OR of the axis constants (e.g. kAxisLinearX | kAxisFreeAngularZ)
   */
  std::function<uint32_t()> get_controlled_axes;

  /**
   * Optional method to get the linear position in the parent frame
   * @return linear position of this frame in the parent frame (m)
   */
  std::function<Vector()> get_linear_position;

  /**
   * Optional method to get the angular position in the parent frame
   * @return quaternion that rotates from this frame to the parent frame
   */
  std::function<Quaternion()> get_angular_position;

  /**
   * Optional method to get the linear velocity in this frame.
   * Automatically derived if get_linear_velocity_in_parent and get_angular_position are provided.
   * @return linear velocity of this frame expressed in itself (m/s)
   */
  std::function<Vector()> get_linear_velocity_in_self;

  /**
   * Optional method to get the linear velocity in the parent frame.
   * Automatically derived if get_linear_velocity_in_self and get_angular_position are provided.
   * @return linear velocity of this frame expressed in parent frame (m/s)
   */
  std::function<Vector()> get_linear_velocity_in_parent;

  /**
   * Optional method to get the angular velocity in this frame.
   * Automatically derived if get_angular_velocity_in_parent and get_angular_position are provided.
   * @return angular velocity of this frame expressed in itself (rad/s)
   */
  std::function<Vector()> get_angular_velocity_in_self;

  /**
   * Optional method to get the angular velocity in the parent frame.
   * Automatically derived if get_angular_velocity_in_self and get_angular_position are provided.
   * @return angular velocity of this frame expressed in parent frame (rad/s)
   */
  std::function<Vector()> get_angular_velocity_in_parent;

  /**
   * Method to get the linear velocity limits in this frame.
   * Required if linear velocity can be controlled, unless get_linear_velocity_limits_in_parent
   * and get_angular_position are also provided (in which case it will be automatically derived).
   * @return linear velocity limits of this frame expressed in itself (rad/s)
   */
  std::function<Vector()> get_linear_velocity_limits_in_self;

  /**
   * Method to get the linear velocity limits in the parent frame.
   * Required if linear velocity can be controlled, unless get_linear_velocity_limits_in_self
   * and get_angular_position are also provided (in which case it will be automatically derived).
   * @return linear velocity limits of this frame expressed in parent frame (rad/s)
   */
  std::function<Vector()> get_linear_velocity_limits_in_parent;

  /**
   * Method to get the angular velocity limits in this frame.
   * Required if angular velocity can be controlled, unless get_angular_velocity_limits_in_parent
   * and get_angular_position are also provided (in which case it will be automatically derived).
   * @return angular velocity limits of this frame expressed in itself (rad/s)
   */
  std::function<Vector()> get_angular_velocity_limits_in_self;

  /**
   * Method to get the angular velocity limits in the parent frame.
   * Required if angular velocity can be controlled, unless get_angular_velocity_limits_in_self
   * and get_angular_position are also provided (in which case it will be automatically derived).
   * @return angular velocity limits of this frame expressed in parent frame (rad/s)
   */
  std::function<Vector()> get_angular_velocity_limits_in_parent;

  /**
   * Optional method to set the desired linear acceleration of this frame in itself.
   * Indicate which axes of motion can be controlled in get_controlled_axes.
   * @param acceleration_ratio - linear acceleration ratio of this frame in itself
   */
  std::function<void(Vector acceleration_ratio)> set_linear_accel_ratio_in_self;

  /**
   * Optional method to set the desired linear velocity of this frame in itself.
   * Automatically derived if set_linear_velocity_in_parent and get_angular_position are provided.
   * Indicate which axes of motion can be controlled in get_controlled_axes.
   * @param velocity_in_self - linear velocity in this frame (m/s)
   */
  std::function<void(Vector velocity_in_self)> set_linear_velocity_in_self;

  /**
   * Optional method to set the desired linear velocity of this frame in its parent frame.
   * Automatically derived if set_linear_velocity_in_self and get_angular_position are provided.
   * Indicate which axes of motion can be controlled in get_controlled_axes.
   * @param velocity_in_parent - linear velocity in the parent frame (m/s)
   */
  std::function<void(Vector velocity_in_parent)> set_linear_velocity_in_parent;

  /**
   * Optional method to set the desired angular velocity of this frame in itself.
   * Automatically derived if set_angular_velocity_in_parent and get_angular_position are provided.
   * Indicate which axes of motion can be controlled in get_controlled_axes.
   * @param velocity_in_self - angular velocity in this frame (rad/s)
   */
  std::function<void(Vector velocity_in_self)> set_angular_velocity_in_self;

  /**
   * Optional method to set the desired angular velocity of this frame in its parent frame.
   * Automatically derived if set_angular_velocity_in_self and get_angular_position are provided.
   * Indicate which axes of motion can be controlled in get_controlled_axes.
   * @param velocity_in_parent - angular velocity in the parent frame (rad/s)
   */
  std::function<void(Vector velocity_in_parent)> set_angular_velocity_in_parent;

  /**
   * Optional method to set the desired angular position.
   * Note that some axes of motion may be ignored as indicated by get_controlled_axes.
   * @param rotation_to_parent - quaternion rotation to the parent frame
   */
  std::function<void(Quaternion rotation_to_parent)> set_angular_position;

  /// Bit flag indicating support for linear motion along the x-axis
  static constexpr uint32_t kAxisLinearX = 1;

  /// Bit flag indicating support for linear motion along the y-axis
  static constexpr uint32_t kAxisLinearY = 1 << 1;

  /// Bit flag indicating support for linear motion along the z-axis
  static constexpr uint32_t kAxisLinearZ = 1 << 2;

  /// Bit flag indicating support for free (holonomic) angular motion about the x-axis
  static constexpr uint32_t kAxisFreeAngularX = 1 << 3;

  /// Bit flag indicating support for free (holonomic) angular motion about the y-axis
  static constexpr uint32_t kAxisFreeAngularY = 1 << 4;

  /// Bit flag indicating support for free (holonomic) angular motion about the z-axis
  static constexpr uint32_t kAxisFreeAngularZ = 1 << 5;

  /// Bit flag indicating support for steered (non-holonomic) angular motion about the x-axis
  static constexpr uint32_t kAxisSteeredAngularX = 1 << 6;

  /// Bit flag indicating support for steered (non-holonomic) angular motion about the y-axis
  static constexpr uint32_t kAxisSteeredAngularY = 1 << 7;

  /// Bit flag indicating support for steered (non-holonomic) angular motion about the z-axis
  static constexpr uint32_t kAxisSteeredAngularZ = 1 << 8;

  /// Bit flags for all axes of linear motion
  static constexpr uint32_t kAxesLinear = kAxisLinearX | kAxisLinearY | kAxisLinearZ;

  /// Bit flags for all axes of free (holonomic) angular motion
  static constexpr uint32_t kAxesFreeAngular = kAxisFreeAngularX | kAxisFreeAngularY | kAxisFreeAngularZ;

  /// Bit flags for all axes of steered (non-holonomic) angular motion
  static constexpr uint32_t kAxesSteeredAngular = kAxisSteeredAngularX | kAxisSteeredAngularY | kAxisSteeredAngularZ;

  /// Bit flags for all axes of motion
  static constexpr uint32_t kAxesAll = kAxesLinear | kAxesFreeAngular | kAxesSteeredAngular;

  /// Bit flag indicating no supported motion
  static constexpr uint32_t kAxesNone = 0;
};

}  // namespace kinesis
