/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once
#include "sdk/logger.h"
#include "sdk_beta/notification_client.h"

#include <chrono>
#include <future>

namespace kinesis {

/**
 * @brief Tester class for the Notification Client Implementation.
 *
 * This class provides a suite of tests to validate the functionality and reliability
 * of the Notification Client implementation. It includes tests for notification
 * discovery and button selection to ensure the system behaves as expected under
 * various scenarios.
 */
class NotificationClientImplTester
{
public:
  /**
   * @param node_id The node used to listen for updates to other nodes on the network
   */
  NotificationClientImplTester(std::string_view node_id);

  /**
   * @brief Runs all the tests for the Notification Client Implementation.
   *
   * This includes tests for:
   *   1. Notification discovery
   *   2. Button selection
   *
   * Each of these tests will ensure that the Notification Client behaves
   * as expected under various scenarios.
   */
  void runAllTests();

private:
  /**
   * @brief Tests the node discovery process.
   *
   * This test includes:
   *   1. Creating a test notification.
   *   2. Verifying that all fields in the new notification are accurately reflected.
   *   3. Cancelling the notification and verifying the callback response.
   *
   * @param new_node_id The ID of the new node to be used
   */
  void runNotificationDiscoveryTest(std::string new_node_id);

  /**
   * @brief Tests the button selection functionality.
   *
   * This test verifies the selection and interaction logic of buttons within notifications.
   *
   * @param new_node_id The ID of the new node to be used
   */
  void runSelectButtonTest(std::string new_node_id);

  /**
   * @brief Blocking method that waits for the promise to be fulfilled within the specified timeout.
   *
   * @param[in/out] promise A std::promise that is expecting to be fulfilled.
   * @param timeout The duration to wait before timing out.
   * @return true if the promise is fulfilled within the allotted time
   */
  bool waitForPromise(std::promise<void>& promise, const std::chrono::steady_clock::duration& timeout);

  /**
   * @brief Creates a kinesis node for testing purposes.
   *
   * @param name The name of the node.
   * @param make The make of the node.
   * @param model The model of the node.
   * @return The ID of the created node.
   */
  std::string constructNode(std::string_view name, std::string_view make, std::string_view model);

  /** Logger **/
  static constexpr Logger kLog = Logger("NotificationClientImplTester");

  /** The maximum time allowed for notification data to propagate over the network */
  static constexpr std::chrono::duration kNotificationDataReceiveTimeout = std::chrono::seconds(1);

  /** The node that is being used to listen */
  std::string node_id_;

  /** Notification client **/
  std::shared_ptr<NotificationClient> notification_client_;
};

}  // namespace kinesis
