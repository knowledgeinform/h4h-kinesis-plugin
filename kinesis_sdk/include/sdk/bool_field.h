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
 * Interface object representing a single bool field
 */
class BoolField : public virtual InputField
{
public:
  /**
   * Create a BoolField object
   * @param name - bool field name
   * @param value - bool field value
   * @return pointer to a new BoolField
   */
  static std::shared_ptr<BoolField> create(std::string_view name, bool value);

  /**
   * @brief Returns an event handler for value update requests.
   *
   * As an SDK user, you have full control over the value of this bool field.
   * When an end user or Kinesis would like to modify the field’s value,
   * a "set value" request is sent to this event handler.
   *
   * To handle these requests, register a callback with the event handler:
   *
   * @code
   * my_bool_field->eventSetValue()->setCallback([](bool value) {
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
  virtual std::shared_ptr<EventHandler<void(bool)>> eventSetValue() = 0;

  /**
   * Get the bool field value
   * @return bool
   */
  virtual bool getValue() const = 0;

  /**
   * Set the bool field value
   * @param value - bool field value
   */
  virtual void setValue(bool value) = 0;
};

}  // namespace kinesis
