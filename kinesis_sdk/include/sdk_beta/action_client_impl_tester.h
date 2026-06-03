/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include "sdk/logger.h"
#include "sdk/platform_interface.h"
#include "sdk_beta/action_client.h"

#include <future>

namespace kinesis {

/**
 * ActionClientImplTester
 * @brief This class is responsible for testing the ActionClient implementation.
 *        It provides functionality to construct and destroy nodes, run specific
 *        and comprehensive tests on the action client, and manage the timing of
 *        test actions using promises.
 */
class ActionClientImplTester
{
public:
  /**
   * @brief Constructor for ActionClientImplTester.
   *
   * @param node_id The ID of the node that this tester will be using.
   */
  ActionClientImplTester(std::string_view node_id);

  /**
   * @brief Run all tests for the action client.
   */
  void runAllTests();

  /**
   * @brief Run a specific test for the action client.
   *
   * @param test_node_id The ID of the test node to be used in the test.
   */
  void runActionClientTest(std::string_view test_node_id);

private:
  /**
   * @brief Constructs a node with the given details.
   *
   * @param name The name of the node.
   * @param make The make of the node.
   * @param model The model of the node.
   * @return A string representing the ID of the constructed node.
   */
  std::string constructNode(std::string_view name, std::string_view make, std::string_view model);

  /**
   * @brief Destroys a node given its ID.
   *
   * @param node_id The ID of the node to be destroyed.
   */
  void destroyNode(std::string_view node_id) const;

  /**
   * @brief Waits for a promise to be fulfilled within a specified timeout duration.
   *
   * @param promise The promise to wait for.
   * @param timeout The duration to wait before timing out.
   * @return true if the promise is fulfilled within the timeout, otherwise false.
   */
  bool waitForPromise(std::promise<void>& promise, const std::chrono::steady_clock::duration& timeout) const;

  /** The ID of the current node being used by the tester. */
  std::string node_id_;

  /** A shared pointer to an ActionClient used by the tester. */
  std::shared_ptr<ActionClient> action_client_;

  /** An instance of ActionInterface used for defining the actions the client can perform. */
  ActionInterface action_interface_;

  /** A structure to hold the action data specific to this tester. */
  ActionData tester_data_;

  /** The maximum time allowed for node data to propagate over the network */
  static constexpr std::chrono::duration kActionDataReceiveTimeout = std::chrono::seconds(1);
  static constexpr Logger kLog = Logger("ActionClientImplTester");
};

}  // namespace kinesis
