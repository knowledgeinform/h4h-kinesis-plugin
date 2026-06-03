/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include <functional>

namespace kinesis {

/**
 * @brief Holds a callback for an event, with the ability to enable / disable it to indicate whether the event can be
 * handled at the current time.
 */
template <typename Signature>
class EventHandler
{
public:
  /// Destructor
  virtual ~EventHandler() = default;

  /**
   * @brief Sets the callback that will be triggered when the event occurs
   *
   * Note that only a single callback is supported. The existing callback if one was already registered.
   *
   * @param callback The function to call when the associated event occurs
   */
  virtual void setCallback(std::function<Signature> callback) = 0;

  /**
   * @brief Set whether this handler is currently able to handle the associated event (by default, the event handler is
   * enabled)
   * @param enabled True if the associated event can be handled at the current time
   */
  virtual void setEnabled(bool enabled) = 0;

  /**
   * @brief Get whether this handler is currently able to handle the associated event (by default, the event handler is
   * enabled)
   * @return True if the associated event can be handled at the current time
   */
  virtual bool isEnabled() const = 0;
};

}  // namespace kinesis
