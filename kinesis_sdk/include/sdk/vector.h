/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include <sstream>

namespace kinesis {

/**
 * Container class for a 3D vector
 */
class Vector
{
public:
  double x = 0.0;
  double y = 0.0;
  double z = 0.0;

  /**
   * Construct a zero vector (all elements are zero)
   */
  Vector() = default;

  /**
   * Construct a vector
   * @param input_x
   * @param input_y
   * @param input_z
   */
  Vector(double input_x, double input_y, double input_z);

  /**
   * Get whether this vector and another are equal
   * @param v other vector
   * @return true if equal
   */
  bool operator==(const Vector& v) const;

  /**
   * Get the sum of this and another vector
   * @param v other vector
   * @return Vector
   */
  Vector operator+(const Vector& v) const;

  /**
   * Get the difference between this and another vector
   * @param v other vector
   * @return Vector
   */
  Vector operator-(const Vector& v) const;

  /**
   * Add another vector to this
   * @param v other vector
   * @return Vector&
   */
  Vector& operator+=(const Vector& v);

  /**
   * Scale this vector by a scalar
   * @param s scalar
   * @return Vector&
   */
  Vector& operator*=(double s);

  /**
   * Scale a vector by a scalar
   * @param s scalar
   * @param v vector
   * @return Vector
   */
  friend Vector operator*(double s, Vector v);

  /**
   * Scale a vector by a scalar
   * @param v vector
   * @param s scalar
   * @return Vector
   */
  friend Vector operator*(Vector v, double s);

  /**
   * @brief Get the dot product of this vector with another
   *
   * This is equivalent to:
   * @code
   * Vector a(1,2,3);
   * Vector b(4,5,6);
   * Vector dot_product = (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
   * @endcode
   *
   * @param v - second vector
   * @return dot product
   */
  double dotProduct(const Vector& v);

  /**
   * @brief Get the element-wise product of this vector with another
   *
   * This is equivalent to:
   * @code
   * Vector a(1,2,3);
   * Vector b(4,5,6);
   * Vector element_product(a.x * b.x, a.y * b.y, a.z * b.z);
   * @endcode
   *
   * @param v - second vector
   * @return element-wise product
   */
  Vector elementProduct(const Vector& v);

  /**
   * Get the magnitude (2-norm) of this vector
   * @return magnitude
   */
  double getMagnitude() const;

  /**
   * Support for output stream operator.
   *
   * Example
   *  - kLog.v << "My vector = " << vector
   *
   * @param os
   * @param v
   * @return a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const Vector& v)
  {
    return os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
  }
};

}  // namespace kinesis
