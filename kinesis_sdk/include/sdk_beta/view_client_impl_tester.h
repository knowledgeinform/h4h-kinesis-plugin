/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include "sdk/frame_interface.h"
#include "sdk/location_interface.h"
#include "sdk/logger.h"
#include "sdk/platform_interface.h"
#include "sdk_beta/view_client.h"

#include <future>

namespace kinesis {

class ViewClientImplTester
{
public:
  /**
   * @brief Constructor for the ViewClientImplTester.
   *
   * @param node_id A string view representing the node identifier.
   */
  explicit ViewClientImplTester(std::string_view node_id);

  /// Runs all the View Client tests
  void runAllTests();

  /// Runs the View Client tests
  void runViewClientTests(std::string_view test_node_id);

private:
  /**
   * @brief Creates a Kinesis node for testing purposes.
   *
   * @param name The name of the node.
   * @param make The make of the node.
   * @param model The model of the node.
   * @return The ID of the created node.
   */
  std::string constructNode(std::string_view name, std::string_view make, std::string_view model);

  /**
   * @brief Destroys the node with the given node ID.
   *
   * @param node_id The ID of the node to be destroyed.
   */
  void destroyNode(std::string_view node_id) const;

  /**
   * @brief Blocking method that waits for the promise to be fulfilled within the specified timeout.
   *
   * @param promise A std::promise that is expecting to be fulfilled.
   * @param timeout The duration to wait before timing out.
   * @return true if the promise is fulfilled within the allotted time
   */
  bool waitForPromise(std::promise<void>& promise, const std::chrono::steady_clock::duration& timeout) const;

  /// The node that is being used to listen
  std::string node_id_;

  /// View Client under test
  std::shared_ptr<ViewClient> view_client_;

  /// Interface to be checked or modified
  ViewInterface view_interface_;

  /// These objects help to configuring the test environment and typically won't be interacted with much.
  kinesis::Location cfov_;
  LocationInterface location_interface_;
  FrameInterface gimbal_frame_interface_;

  /// The maximum time allowed for view data to propagate over the network
  static constexpr std::chrono::duration kViewDataReceiveTimeout = std::chrono::seconds(1);

  /// Logger
  static constexpr Logger kLog = Logger("viewClientImplTester");

  /// URLs to test updating view in the client
  std::string url_primary_;
  std::string url_secondary_;

  /// boolean to switch which URL is referenced by the view interface
  bool is_primary_view_selected_;

  static constexpr char kTestViewName[] = "Down(EO)";
};

}  // namespace kinesis
