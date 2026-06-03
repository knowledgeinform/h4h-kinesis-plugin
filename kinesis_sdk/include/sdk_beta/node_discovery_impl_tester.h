/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include "sdk/logger.h"
#include "sdk/platform_interface.h"
#include "sdk_beta/node_discovery.h"

#include <future>

namespace kinesis {

/**
 * Automated tests for the NodeDiscovery class
 */
class NodeDiscoveryImplTester
{
public:
  /**
   * @param node_id The node used to listen for updates to other nodes on the network
   */
  NodeDiscoveryImplTester(std::string_view node_id);

  void runAllTests();

  /**
   * Tests node discovery by
   *   1. Creating a test node
   *   2. Using the {NodeDiscovery} class to discover the test node
   *   3. Updating the name of the node
   *   4. Using the {NodeDiscovery} class to detect the updated name
   *
   * Also tests {getNodeData}, {startListening}, {stopListening}, and {isListening}
   */
  void runNodeDiscoveryTest();

private:
  static constexpr Logger kLog = Logger("NodeDiscoveryImplTester");
  /** The maximum time allowed for node data to propagate over the network */
  static constexpr std::chrono::duration kNodeDataReceiveTimeout = std::chrono::seconds(1);
  /** The node that is being used to listen for other nodes on the network */
  std::string node_id_;
  std::shared_ptr<NodeDiscovery> node_discovery_;

  /**
   * @brief Creates a kinesis node for testing purposes.
   *
   * @param name The name of the node.
   * @param make The make of the node.
   * @param model The model of the node.
   * @return The ID of the created node.
   */
  std::string constructNode(std::string_view name, std::string_view make, std::string_view model);

  /**
   * @brief Deletes a kinesis node
   * @param node_id The node to delete
   */
  void destroyNode(std::string_view node_id);

  /**
   * @brief Sets the name of the specified node.
   *
   * Note: This takes a hacky approach for setting the name by accessing KinesisNode directly.
   * In the future, once third parties can update the names of their nodes, the test should be
   * reconfigured to only use the public Kinesis SDK interfaces.
   */
  void hackySetNodeName(std::string_view node_id, std::string_view name);

  /**
   * @brief Blocking method that waits for the promise to be fulfilled within the specified timeout.
   *
   * @param[in/out] promise A std::promise that is expecting to be fulfilled.
   * @param timeout The duration to wait before timing out.
   * @return true if the promise is fulfilled within the allotted time
   */
  bool waitForPromise(std::promise<void>& promise, const std::chrono::steady_clock::duration& timeout);
};

}  // namespace kinesis
