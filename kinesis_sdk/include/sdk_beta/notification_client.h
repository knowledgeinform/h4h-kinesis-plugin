/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once
#include "notification_data.h"

#include <functional>
#include <memory>

namespace kinesis {

/**
 * @brief Interface class to interact and receive Kinesis Notifications.
 * To instantiate, populate your config with your callbacks and call the create method.
 *
 * Example usage:
 *
 * @note To generate a user interface node, see @ref NodeFactory
 *
 * @code
 * // Step 1: Generate a user interface node using the NodeFactory
 * auto user_node_id = ...
 *
 * // Step 2: Use the user interface node to begin discovering other nodes on the network
 * kinesis::NotificationClient::Config config;
 * config.on_post = [](const NotificationData& notification) {
 *   std::cout << "Received notification with id: " << notification.id << std::endl;
 * };
 *
 * config.on_canceled = [](const std::string notification_id) {
 *   std::cout << "Canceled notification with id: " << notification.id << std::endl;
 * };
 *
 * auto notification_client = NotificationClient::create(node_id_, config);
 * @endcode
 */
class NotificationClient
{
public:
  struct Config
  {
    /// Function to call when notification data is discovered
    std::function<void(const NotificationData&)> on_post = nullptr;

    /// Function to call when notification data is updated
    std::function<void(const std::string&)> on_canceled = nullptr;
  };

  /**
   * @brief Constructor
   *
   * Create a new NotificationClient by using the existing network node.
   * The existing node provides access to the network.
   *
   * @param node_id The node that will be used to listen for other nodes on the network.
   * @param config The configuration containing optional callbacks
   * @return A shared pointer to the created NotificationClient instance.
   */
  static std::shared_ptr<NotificationClient> create(std::string_view node_id, Config config);

  /**
   * @brief Notify Kinesis that a notification button has been selected by a user.
   *
   * @param notification_id ID of the notification.
   * @param display_name display name of the button that was selected.
   */
  virtual void buttonSelected(const std::string& notification_id, const std::string& display_name) = 0;
};

}  // namespace kinesis
