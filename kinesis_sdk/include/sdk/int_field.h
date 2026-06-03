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
 * Interface object representing a single int field
 */
class IntField : public virtual InputField
{
public:
  /**
   * Create an IntField object
   * @param name - int field name
   * @param value - int field value
   * @return pointer to a new IntField
   */
  static std::shared_ptr<IntField> create(std::string_view name, int value);

  /**
   * Create an IntField object
   * @param name - int field name
   * @param value - int field value (will be rounded to nearest integer)
   * @return pointer to a new IntField
   */
  static std::shared_ptr<IntField> create(std::string_view name, double value);

  /**
   * @brief Returns an event handler for value update requests.
   *
   * As an SDK user, you have full control over the value of this int field.
   * When an end user or Kinesis would like to modify the field’s value,
   * a "set value" request is sent to this event handler.
   *
   * To handle these requests, register a callback with the event handler:
   *
   * @code
   * my_int_field->eventSetValue()->setCallback([](int value) {
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
  virtual std::shared_ptr<EventHandler<void(int)>> eventSetValue() = 0;

  /**
   * Get the int field value
   * @return int
   */
  virtual int getValue() const = 0;

  /**
   * Set the int field value
   * @param value - int field value
   */
  virtual void setValue(int value) = 0;

  /**
   * Get the minimum value for this int field
   * @return minimum value
   */
  virtual std::optional<int> getMinValue() const = 0;

  /**
   * Get the maximum value for this int field
   * @return maximum value
   */
  virtual std::optional<int> getMaxValue() const = 0;

  /**
   * Set the min int field value
   * @param value - min int field value
   */
  virtual void setMinValue(std::optional<int> value) = 0;

  /**
   * Set the max int field value
   * @param value - max int field value
   */
  virtual void setMaxValue(std::optional<int> value) = 0;
};

}  // namespace kinesis
