/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include "node_data.h"

#include <functional>
#include <memory>
#include <optional>

namespace kinesis {

/**
 * @brief A class responsible for discovering nodes on the Kinesis network.
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
 * kinesis::NodeDiscovery::Config config;
 * config.on_discovery = [](const kinesis::NodeData& node_data) {
 *     std::cout << "Discovered node: " << node_data.name << std::endl;
 * };
 * config.on_update = [](const kinesis::NodeData& node_data) {
 *     std::cout << "Updated node: " << node_data.name << std::endl;
 * };
 *
 * auto node_discovery = kinesis::NodeDiscovery::create(user_node_id, config);
 * @endcode
 */
class NodeDiscovery
{
public:
  struct Config
  {
    /// Called when a node is discovered on the kinesis network
    std::function<void(const NodeData&)> on_discovery;

    /// Called when the data associated with a node is updated
    std::function<void(const NodeData&)> on_update;
  };

  /**
   * @brief Constructor
   *
   * @param node_id The node that will be used to listen for other nodes on the network.
   * @param config The configuration containing optional callbacks
   * @return A shared pointer to the new NodeDiscovery instance.
   */
  static std::shared_ptr<NodeDiscovery> create(std::string_view node_id, Config config);

  /// Destructor
  virtual ~NodeDiscovery() = default;

  /**
   * @brief Retrieves the NodeData for a specified node.
   *
   * @param node_id The node whose data is to be retrieved.
   * @return The node data or std::nullopt if no matching node was found
   */
  virtual std::optional<NodeData> getNodeData(std::string_view node_id) const = 0;
};

}  // namespace kinesis
