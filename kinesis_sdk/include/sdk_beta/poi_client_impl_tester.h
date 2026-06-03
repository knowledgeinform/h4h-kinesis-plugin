/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once
#include "sdk/logger.h"
#include "sdk/poi_interface.h"
#include "sdk_beta/poi_client.h"

#include <chrono>
#include <future>

namespace kinesis {

class PoiClientImplTester
{
public:
  /**
   * @param node_id The node used to listen for POI updates on the network
   */
  PoiClientImplTester(std::string_view node_id);

  /**
   * Tests POI client and POI interface by
   *   1. Creating a test node and POI interface
   *   2. Create a POI and add it to the interface
   *   3. Using the {PoiClient} class to discover the test POI
   *   4. Updating the name, icon, and location of the POI
   *   5. Using the {PoiClient} class to detect the updated POI
   *   6. Delete the test POI
   *   7. Using the {PoiClient} class to detect the deleted POI
   */
  void runAllTests();

private:
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

  /**
   * @brief Deletes a kinesis node
   * @param node_id The node to delete
   */
  void destroyNode(std::string_view node_id);

  /** Logger **/
  static constexpr Logger kLog = Logger("PoiClientImplTester");

  /** The maximum time allowed for POI data to propagate over the network */
  static constexpr std::chrono::duration kPoiDataReceiveTimeout = std::chrono::seconds(1);

  /** The node that is being used to listen */
  std::string node_id_;

  /** POI client **/
  std::shared_ptr<PoiClient> poi_client_;

  /** POI interface **/
  std::shared_ptr<PoiInterface> poi_interface_;
};

}  // namespace kinesis
