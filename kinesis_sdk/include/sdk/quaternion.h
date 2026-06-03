/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include "sdk/vector.h"

#include <sstream>

namespace kinesis {

/// Container class for a unit quaternion representing a 3D rotation
class Quaternion
{
public:
  double w = 1.0;
  double x = 0.0;
  double y = 0.0;
  double z = 0.0;

  /// Construct an identity quaternion
  Quaternion() = default;

  /**
   * @brief Construct a unit quaternion (will be normalized)
   *
   * @param input_w
   * @param input_x
   * @param input_y
   * @param input_z
   */
  Quaternion(double input_w, double input_x, double input_y, double input_z);

  /**
   * @brief Construct a quaternion equivalent to the given angular displacement
   *
   * @param angular_displacement - angular displacement vector (rad)
   */
  Quaternion(Vector angular_displacement);

  /**
   * @brief Get whether this quaternion and another are equal
   *
   * @param other_quaternion - other quaternion
   * @return true if equal
   */
  bool operator==(const Quaternion& other_quaternion) const;

  /**
   * @brief Multiply this quaternion by another
   *
   * @param other_quaternion - other quaternion
   * @return product
   */
  Quaternion operator*(const Quaternion& other_quaternion) const;

  /**
   * @brief Rotate a vector with this quaternion
   *
   * @param vector_to_rotate - vector before rotation
   * @return vector after rotation
   */
  Vector operator*(const Vector& vector_to_rotate) const;

  /**
   * @brief Get the inverse of a rotation quaternion
   *
   * @return Quaternion
   */
  Quaternion getInverse() const;

  /**
   * @brief Support for output stream operator.
   *
   * Example
   *  - kLog.v << "My quaternion = " << quaternion
   *
   * @param os
   * @param q
   * @return a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const Quaternion& q)
  {
    return os << "(" << q.w << ", " << q.x << ", " << q.y << ", " << q.z << ")";
  }

  /**
   * @brief Get a quaternion that represents a rotation about the x-axis
   *
   * @param angle - magnitude of rotation (rad)
   * @return Quaternion - quaternion rotation to new frame (rotated about its x-axis)
   */
  static Quaternion getRotationX(double angle);

  /**
   * @brief Get a quaternion that represents a rotation about the y-axis
   *
   * @param angle - magnitude of rotation (rad)
   * @return Quaternion - quaternion rotation to new frame (rotated about its y-axis)
   */
  static Quaternion getRotationY(double angle);

  /**
   * @brief Get a quaternion that represents a rotation about the z-axis
   *
   * @param angle - magnitude of rotation (rad)
   * @return Quaternion - quaternion rotation to new frame (rotated about its z-axis)
   */
  static Quaternion getRotationZ(double angle);

  /**
   * @brief Get the quaternion that rotates to ENU (East-North-Up) from NED (North-East-Down)
   *
   * @return Quaternion - rotation to ENU from NED
   */
  static Quaternion getRotationEnuFromNed();

  /**
   * @brief Get the quaternion that rotates to FRD (Forward-Right-Down) from FLU (Forward-Left-Up)
   *
   * @return Quaternion - rotation to FRD from FLU
   */
  static Quaternion getRotationFrdFromFlu();
};

}  // namespace kinesis
