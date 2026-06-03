/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include "view_data.h"

namespace kinesis {

/**
 * @brief A class responsible for discovering views on the network. To learn more about Views, see @ref ViewInterface.
 *
 * Example discovery:
 *
 * @note To generate a user interface node, see @ref NodeFactory
 *
 * @code
 * // Step 1: Generate a user interface node using the NodeFactory
 * auto user_node_id = node_factory.constructNode(...);
 *
 * // Step 2: Use the View Client to begin discovering the views of other nodes on the network
 * kinesis::ViewClient::Config config;
 *
 * config.on_discovery = [](const kinesis::ViewData& data) {
 *     std::cout << "Discovered view: " << data.view_id << std::endl;
 * };
 *
 * config.on_update = [](const kinesis::ViewData& data) {
 *     std::cout << "Received view update" << std::endl;
 * };
 *
 * auto view_client = kinesis::ViewClient::create(user_node_id, config);
 * @endcode
 */
class ViewClient
{
public:
  /// Provided at construction time to configure the client
  struct Config
  {
    /// Called when a view is discovered
    std::function<void(const ViewData&)> on_discovery;
    /// Called when a view is updated
    std::function<void(const ViewData&)> on_update;
  };

  /**
   * @brief Constructor
   *
   * @param node_id - The node that will be used to listen to views on the network
   * @param config - The configuration containing optional callbacks
   * @return A shared pointer to the created ViewClient instance
   */
  static std::shared_ptr<ViewClient> create(std::string_view node_id, const Config& config);

  /// Destructor
  virtual ~ViewClient() = default;

  /**
   * @brief Retrieves the ViewData for the specified frame
   *
   * @param view_id The view to retrieve
   * @return The view data or std::nullopt if no matching frame was found
   */
  virtual std::optional<ViewData> getViewData(std::string_view view_id) const = 0;
};

}  // namespace kinesis
