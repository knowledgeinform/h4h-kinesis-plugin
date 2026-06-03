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
 * Interface object representing a single string field
 */
class StringField : public virtual InputField
{
public:
  /**
   * Create a StringField object
   * @param name - string field name
   * @param value - string field value
   * @return pointer to a new StringField
   */
  static std::shared_ptr<StringField> create(std::string_view name, std::string_view value);

  /**
   * @brief Returns an event handler for requests to change the value of this field.
   *
   * As an SDK user, you have full control over the value of this string field.
   * When an end user or Kinesis would like to modify the field’s value,
   * a "set value" request is sent to this event handler.
   *
   * To handle these requests, register a callback with the event handler:
   *
   * @code
   * my_string_field->eventSetValue()->setCallback([](std::string value) {
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
  virtual std::shared_ptr<EventHandler<void(std::string)>> eventSetValue() = 0;

  /**
   * Get the string field value
   * @return string
   */
  virtual std::string getValue() const = 0;

  /**
   * Set the string field value
   * @param value - string field value
   */
  virtual void setValue(std::string_view value) = 0;
};

}  // namespace kinesis
