/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include "input_field.h"
#include "sdk/event_handler.h"

#include <memory>
#include <optional>
#include <string>

namespace kinesis {

/**
 * Interface object representing a single double field
 */
class DoubleField : public virtual InputField
{
public:
  /**
   * Create a DoubleField object
   * @param name - double field name
   * @param value - double field value
   * @return pointer to a new DoubleField
   */
  static std::shared_ptr<DoubleField> create(std::string_view name, double value);

  /**
   * @brief Returns an event handler for value update requests.
   *
   * As an SDK user, you have full control over the value of this double field.
   * When an end user or Kinesis would like to modify the field’s value,
   * a "set value" request is sent to this event handler.
   *
   * To handle these requests, register a callback with the event handler:
   *
   * @code
   * my_double_field->eventSetValue()->setCallback([](double value) {
   *     log.i << "Kinesis is requesting the value change to: " << value;
   * });
   * @endcode
   *
   * If no callback is registered, Kinesis assumes the field is read-only
   * and its value cannot be changed.
   *
   * @see EventHandler documentation for more details.
   *
   * @return A shared pointer to an EventHandler
   */
  virtual std::shared_ptr<EventHandler<void(double)>> eventSetValue() = 0;

  /**
   * Get the double field value
   * @return double
   */
  virtual double getValue() const = 0;

  /**
   * Set the double field value
   * @param value - double field value
   */
  virtual void setValue(double value) = 0;

  /**
   * Get the minimum value for this double field
   * @return minimum value
   */
  virtual std::optional<double> getMinValue() const = 0;

  /**
   * Get the maximum value for this double field
   * @return maximum value
   */
  virtual std::optional<double> getMaxValue() const = 0;

  /**
   * Set the min double field value
   * @param value - min double field value
   */
  virtual void setMinValue(std::optional<double> value) = 0;

  /**
   * Set the max double field value
   * @param value - max double field value
   */
  virtual void setMaxValue(std::optional<double> value) = 0;
};

}  // namespace kinesis
