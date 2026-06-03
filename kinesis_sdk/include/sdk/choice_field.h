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
 * Interface object representing a single choice field
 */
class ChoiceField : public virtual InputField
{
public:
  /**
   * Create a ChoiceField object
   * @param name - choice field name
   * @param choice - choice field value
   * @param choices - vector of strings to represent the available choices
   * @return pointer to a new ChoiceField
   */
  static std::shared_ptr<ChoiceField> create(std::string_view name,
                                             std::string_view choice,
                                             const std::vector<std::string>& choices);

  /**
   * @brief Returns an event handler for value update requests.
   *
   * As an SDK user, you have full control over the value of this choice field.
   * When an end user or Kinesis would like to modify the field’s value,
   * a "set value" request is sent to this event handler.
   *
   * To handle these requests, register a callback with the event handler:
   *
   * @code
   * my_choice_field->eventSetValue()->setCallback([](std::string value) {
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
   * Get the choice field value
   * @return string
   */
  virtual std::string getValue() const = 0;

  /**
   * Set the choice field value
   * @param value - choice field value
   */
  virtual void setValue(std::string_view value) = 0;

  /**
   * Get the possible choices of this choice
   * @return vector of strings
   */
  virtual std::vector<std::string> getChoices() const = 0;
};

}  // namespace kinesis
