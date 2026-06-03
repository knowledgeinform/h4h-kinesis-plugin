/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include "poi_data.h"

#include <functional>
#include <memory>
#include <optional>

namespace kinesis {

/**
 * @brief A class responsible for discovering Points of Interest (POI's). To learn more about POI's, see @ref
 * PoiInterface.
 *
 * Example discovery:
 *
 * @note To generate a user interface node, see @ref NodeFactory
 *
 * @code
 * // Step 1: Generate a user interface node using the NodeFactory
 * auto user_node_id = ...
 *
 * // Step 2: Use the user interface node to begin discovering POI's from other nodes on the network
 * kinesis::PoiClient::Config config;
 *
 * config.on_discovery = [](const kinesis::PoiData& data) {
 *     std::cout << "Discovered POI: " << data.name << std::endl;
 * };
 *
 * config.on_update = [](const kinesis::PoiData& data) {
 *     std::cout << "Received POI update" << std::endl;
 * };
 *
 * config.on_delete = [](const kinesis::PoiData& data) {
 *     std::cout << "POI was deleted" << std::endl;
 * };
 *
 * auto poi_client = kinesis::PoiClient::create(user_node_id, config);
 * @endcode
 */
class PoiClient
{
public:
  /// Provided at construction time to configure the client
  struct Config
  {
    /// Called when a POI is discovered
    std::function<void(const PoiData&)> on_discovery;

    /// Called when the data associated with a POI changes
    std::function<void(const PoiData&)> on_update;

    /// Called when a POI is deleted
    std::function<void(const PoiData&)> on_delete;
  };

  /**
   * @brief Constructor
   *
   * @param node_id - the node that will be used to listen to poi's on the network
   * @param config - The configuration containing optional callbacks
   * @return A shared pointer to the created PoiClient instance.
   */
  static std::shared_ptr<PoiClient> create(std::string_view node_id, Config config);

  /**
   * @brief Retrieves the PoiData for the specified poi
   *
   * @param poi_id The POI to retrieve
   * @return The POI data or std::nullopt if no matching POI was found
   */
  virtual std::optional<PoiData> getPoiData(std::string_view poi_id) const = 0;

  /// Destructor
  virtual ~PoiClient() = default;
};

}  // namespace kinesis
