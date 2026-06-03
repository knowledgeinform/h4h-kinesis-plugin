/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#include <cstdint>
#include <vector>

#pragma once

namespace kinesis {

/**
 * @brief Represents which axes of a coordinate frame can be controlled
 *
 * This struct is used to determine which degrees of freedom (DOF) in a given coordinate
 * frame are controllable. It includes linear and angular components, with angular
 * control further divided into "free" and "steered" types.
 *
 * Each axis is represented as a boolean flag indicating whether it can be controlled.
 */
struct ControlledAxes
{
  /// Linear motion along the x axis can be controlled
  bool linear_x = false;

  /// Linear motion along the y axis can be controlled
  bool linear_y = false;

  /// Linear motion along the z axis can be controlled
  bool linear_z = false;

  /// Free angular motion along the x axis can be controlled
  bool angular_free_x = false;

  /// Free angular motion along the y axis can be controlled
  bool angular_free_y = false;

  /// Free angular motion along the z axis can be controlled
  bool angular_free_z = false;

  /// Steered angular motion along the x axis can be controlled
  bool angular_steered_x = false;

  /// Steered angular motion along the y axis can be controlled
  bool angular_steered_y = false;

  /// Steered angular motion along the z axis can be controlled
  bool angular_steered_z = false;

  bool operator==(const ControlledAxes& other) const
  {
    return linear_x == other.linear_x && linear_y == other.linear_y && linear_z == other.linear_z &&
           angular_free_x == other.angular_free_x && angular_free_y == other.angular_free_y &&
           angular_free_z == other.angular_free_z && angular_steered_x == other.angular_steered_x &&
           angular_steered_y == other.angular_steered_y && angular_steered_z == other.angular_steered_z;
  }
};

}  // namespace kinesis
