/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */
#pragma once

#include <chrono>
#include <cstdarg>
#include <functional>
#include <memory>
#include <optional>
#include <string>

namespace kinesis {

/**
 * @brief A class representing a user-facing notification
 *
 * To construct a notification, use the #build function. After configuring the notification, send it
 * to users via #postLocally or #broadcast.
 *
 * Example usage:
 *
 * @code
 * auto notification = kinesis::Notification::build(kinesis::Notification::Severity::kWarning,
 *                                  "node123",
 *                                  "Battery has %d minutes remaining",
 *                                  minutes_remaining);
 * notification->setDescription("This is a warning notification");
 * notification->addButton("Return Home", []() {
 *   // return home button clicked!
 * });
 * notification->broadcast();
 * @endcode
 */
class Notification
{
public:
  enum Severity
  {
    kStatus,
    kWarning,
    kError,
    kCritical
  };

  /**
   * Build a new Kinesis Notification
   *
   * @param severity - Severity of the notification. Should use one of the constants defined in this header.
   * @param sender_node_id - Sender Node ID value provided at node creation time
   * @param title - Title to be displayed
   * @param ... - Variable arguments for printf style formatting of the title
   * @return std::shared_ptr<Notification>
   */
  static std::shared_ptr<Notification>
  build(Severity severity, const std::string& sender_node_id, std::string title, ...);

  /**
   * Stop showing the specified notification to the user
   *
   * @param notification_id - ID of notification to be cancelled
   * @param sender_node_id - Sender Node ID value provided at node creation time
   */
  static void cancel(const std::string& notification_id, const std::string& sender_node_id);

  /// Destructor
  virtual ~Notification() = default;

  /**
   * Set the title
   *
   * @param title
   * @param ... - Variable arguments for printf style formatting of the title
   * @return reference to this
   */
  virtual Notification& setTitle(std::string title, ...) = 0;

  /**
   * Set the description
   *
   * @param description - description
   * @param ... - Variable arguments for printf style formatting of the description
   * @return reference to this
   */
  virtual Notification& setDescription(std::string description, ...) = 0;

  /**
   * Set the decay time for this notification
   *
   * @param decay_time  - The amount of time before the notification disappears or nullopt for persistent notification
   * @return reference to this
   */
  virtual Notification& setDecayTime(std::optional<std::chrono::steady_clock::duration> decay_time) = 0;

  /**
   * Add a button to this notification. Limited to a maximum of 3.
   *
   * @param name  - user-facing name of the button
   * @param on_click - function to call when the button is clicked
   * @return reference to this
   */
  virtual Notification& addButton(const std::string& name, std::function<void()> on_click) = 0;

  /**
   * Post this notification to the Kinesis UI running on the same device. This will not broadcast to any other devices
   * on the network. Do not use this if your implementation will not run on the same device as the Kinesis UI or your
   * notification will not be seen.
   * @return ID of this notification
   */
  virtual std::string postLocally() = 0;

  /**
   * Broadcast this notification to the kinesis network.
   * @return ID of this notification
   */
  virtual std::string broadcast() = 0;

  /**
   * Post this notification to the Kinesis Node specified. Only that node (if found) will receive
   * the notification
   *
   * @param node_id UUID of the kinesis node to send the notification to
   * @return ID of this notification
   */
  virtual std::string postToNode(const std::string& node_id) = 0;

  /**
   * Get the ID of this notification
   * @return std::string
   */
  virtual std::string getId() const = 0;

protected:
  Notification() = default;
};

}  // namespace kinesis
