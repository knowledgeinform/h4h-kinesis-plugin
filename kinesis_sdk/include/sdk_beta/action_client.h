/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include "action_data.h"

#include <memory>
#include <optional>

namespace kinesis {

/**
 * @brief A class responsible for discovering and triggering Actions. To learn more about Actions, see @ref
 * ActionInterface.
 *
 * Example discovery:
 *
 * @note To generate a user interface node, see @ref NodeFactory
 *
 * @code
 * // Step 1: Generate a user interface node using the NodeFactory
 * auto user_node_id = ...
 *
 * // Step 2: Use the user interface node to begin discovering actions from other nodes on the network
 * kinesis::ActionClient::Config config;
 *
 * config.on_discovery = [](const kinesis::ActionData& data) {
 *     std::cout << "Discovered action: " << data.name << std::endl;
 * };
 *
 * config.on_update = [](const kinesis::ActionData& data) {
 *     std::cout << "Received action update" << std::endl;
 * };
 *
 * auto action_client = kinesis::ActionClient::create(user_node_id, config);
 * @endcode
 *
 * Now let's start one of the actions we discovered
 * @code
 * // provide the id of the action you would like to start
 * action_client->startAction(action_id);
 * @endcode
 */
class ActionClient
{
public:
  /// Provided at construction time to configure the client
  struct Config
  {
    /// Called when an action is discovered
    std::function<void(const ActionData&)> on_discovery;

    /// Called when the data associated with an action changes
    std::function<void(const ActionData&)> on_update;
  };

  /**
   * @brief Constructor
   *
   * @param node_id The node that will be used to listen to the network.
   * @param config The configuration containing optional callbacks
   * @return A shared pointer to the new ActionClient instance.
   */
  static std::shared_ptr<ActionClient> create(std::string_view node_id, const Config& config);

  /// Destructor
  virtual ~ActionClient() = default;

  /**
   * @brief Retrieve action data for the specified action
   *
   * @param action_id The action to retrieve
   * @return The action data or std::nullopt if the no matching action was found
   */
  virtual std::optional<ActionData> getActionData(std::string_view action_id) const = 0;

  /**
   * @brief Retrieve the list of actions associated with a specific node
   *
   * @param node_id The node containing the actions
   * @return A vector of the Actions that belong to the specified node
   */
  virtual std::vector<ActionData> getActionsForNode(std::string_view node_id) const = 0;

  /**
   * @brief Start an action
   *
   * @param action_id The action to start
   * @return True if start request was sent. This is NOT a guarantee that the request was successful.
   * The request will only be sent if the action can currently be started and this node has permission to start it.
   */
  virtual bool startAction(std::string_view action_id) const = 0;

  /**
   * @brief Stop / cancel a currently active action
   *
   * @param action_id The action to stop
   * @return True if the stop request was sent. Note that this does NOT guarantee that the request was successful. The
   * request will only be sent if the action can currently be stopped and this node has the permission to stop it.
   */
  virtual bool stopAction(std::string_view action_id) const = 0;
};
}  // namespace kinesis
