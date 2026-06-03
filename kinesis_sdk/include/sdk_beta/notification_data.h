/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include "sdk/notification.h"

#include <chrono>
#include <string>

namespace kinesis {

/**
 * Struct to represent a notification
 */
struct NotificationData
{
  std::string sender_node_id;
  std::string notification_id;
  std::string title;
  std::string description;
  Notification::Severity severity;
  std::chrono::steady_clock::duration timeout;
  std::vector<std::string> buttons;

  bool operator==(const NotificationData& other) const;
  bool operator!=(const NotificationData& other) const;
};

}  // namespace kinesis
