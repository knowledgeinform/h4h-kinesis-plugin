/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include "geofence_interface.h"
#include "sdk/action_interface.h"
#include "sdk/frame_interface.h"
#include "sdk/lifecycle_interface.h"
#include "sdk/location_interface.h"
#include "sdk/mission_interface.h"
#include "sdk/navigation_interface.h"
#include "sdk/parameter_group_interface.h"
#include "sdk/platform_interface.h"
#include "sdk/poi_interface.h"
#include "sdk/property_interface.h"
#include "sdk/ratio_interface.h"
#include "sdk/touch_interface.h"
#include "sdk/view_interface.h"
#include "sdk_beta/confirmed_action_interface.h"

#include <memory>
#include <string>
#include <vector>

namespace kinesis {

// Forward Declarations
class MissionModuleInterface;
class FieldGroup;

/**
 * @brief Factory used for construction of kinesis nodes. Nodes represent vehicles, user interfaces, etc. and are
 * published over the kinesis network.
 *
 * Example usage:
 * @code
 * kinesis::NodeFactory factory("[my integrator id here]");
 * PlatformInterface platform;
 * platform.name = "My User Interface";
 * platform.make = "My Make";
 * platform.model = "My Model";
 * factory.addPlatform(platform);
 * std::string user_node_id = factory.constructNode();
 * @endcode
 *
 * @note To discover nodes on the network, see @ref NodeDiscovery
 */
class NodeFactory
{
public:
  /// ID returned by constructNode() upon failure
  static constexpr char kConstructionFailure[] = "INVALID";

  /**
   * Constructor
   * @param integrator_id - ID of the integrator
   */
  explicit NodeFactory(const std::string& integrator_id);

  /// Destructor
  virtual ~NodeFactory();

  /**
   * Destroy a Node
   * Call this before destroying any objects that the interface methods point to.
   * @param node_id - the ID of the Node (returned by constructNode()) to be destroyed
   */
  static void destroyNode(const std::string& node_id);

  /**
   * Add the platform interface. This call is mandatory and must be the first call done
   * on the factory to establish the SDK entity.
   * @param platform_interface - mandatory interface for status and control of the platform frame
   * (x: forward, y: left, z: up) with respect to the local-level frame (x: east, y: north, z: up)
   * @return - self reference
   */
  NodeFactory& addPlatform(const PlatformInterface& platform_interface);

  /**
   * Attempts to construct the resulting agent from the components provided.
   * @param lifecycle_interface - Optional interface that provides callbacks for node lifecycle events. Use to ensure
   * the interface implementations are initialized and destroyed in accordance with the node lifecycle. The shared
   * pointer allows the Kinesis node to maintain ownership of the object you pass in, preventing it from being destroyed
   * until the node is no longer using the interfaces you provided.
   * @return Agent_ID of the constructed agent, kConstructionFailure otherwise
   */
  std::string constructNode(std::shared_ptr<LifecycleInterface> lifecycle_interface = nullptr) const;

  /**
   * Overloaded version of method above which accepts node id as an additional argument
   * to construct an agent from a stored NodeSetup item created with a Plugin's NodeSetupActivity.
   * @param node_id the unique identifier for the node. Either retrieved from a NodeSetup.nodeId or
   * passed as an empty string from the overloaded constructNode function above.
   * @param lifecycle_interface - Optional interface that provides callbacks for node lifecycle events. Use to ensure
   * the interface implementations are initialized and destroyed in accordance with the node lifecycle. The shared
   * pointer allows the Kinesis node to maintain ownership of the object you pass in, preventing it from being destroyed
   * until the node is no longer using the interfaces you provided.
   * @return Agent_ID of the constructed agent, kConstructionFailure otherwise
   */
  std::string constructNode(const std::string& node_id,
                            std::shared_ptr<LifecycleInterface> lifecycle_interface = nullptr) const;

  /**
   * Provides the error string from most recent construction failure.
   * @return string output from most recent error
   */
  std::string getConstructionFailure() const;

  /**
   * Adds a video view to the established platform.
   * This function is OPTIONAL to construct an agent.
   * @param view_interface - mandatory interface for status and control of the view
   * @return - self reference
   */
  NodeFactory& addView(const ViewInterface& view_interface);

  /**
   * Adds several video views to the established platform.
   * This function is OPTIONAL to construct an agent.
   * @param view_interface_vector - list of mandatory interfaces for status and control of the views
   * @return - self reference
   */
  NodeFactory& addViews(const std::vector<ViewInterface>& view_interface_vector);

  /**
   * Adds the interface for a Action command to the constructor.
   * This function is OPTIONAL to construct an agent.
   * @param action_interface - mandatory, interface to action controls and status
   * @return - self reference
   */
  NodeFactory& addAction(const ActionInterface& action_interface);

  /**
   * Adds several interfaces for Action commands to the constructor.
   * This function is OPTIONAL to construct an agent.
   * @param action_interface_vector - list of mandatory, interface to action controls and status
   * @return - self reference
   */
  NodeFactory& addActions(const std::vector<ActionInterface>& action_interface_vector);

  /**
   * Add a field group to the node.
   * This function is OPTIONAL to construct an agent.
   * @param field_group - a single parameter page
   * @return - self reference
   */
  NodeFactory& addSettings(std::shared_ptr<FieldGroup> field_group);

  /**
   * Adds a POI interface to the node.
   * This function is OPTIONAL to construct an agent.
   * @param poi_interface - POI interface
   * @return - self reference
   */
  NodeFactory& addPoiInterface(const std::shared_ptr<PoiInterface> poi_interface);

  /**
   * Add a coordinate frame to the node.
   * This function is OPTIONAL.
   * @param frame_interface - frame interface
   * @return - self reference
   */
  NodeFactory& addFrame(const FrameInterface& frame_interface);

  /**
   * Add a geofence associated with this platform
   * @param geofence - geofence interface
   * @return - self reference
   */
  NodeFactory& addGeofence(const std::shared_ptr<GeofenceInterface>& geofence);

  /**
   * Adds a mission module to the application.  Mission Modules can only appear on a Kinesis UI
   * that is physically co-located with the application.  IE. On the same EUD or on the KxM
   * connected to the EUD.  The unique ID provided to the Library Startup (constructor)
   * is used to validate this access.
   * Each node may have only 1 mission module TODO: remove this limitation (as long as names are unique)
   * @param mission_module - shared pointer to the mission module interface
   * @return - self reference
   */
  NodeFactory& addMissionModule(std::shared_ptr<MissionModuleInterface> mission_module);

  /**
   * Add a location associated with this platform
   * @param location_interface - location interface
   * @return - self reference
   */
  NodeFactory& addLocation(const LocationInterface& location_interface);

  /**
   * Add a set of locations associated with this platform
   * @param location_interfaces - vector of location interfaces
   * @return - self reference
   */
  NodeFactory& addLocations(const std::vector<LocationInterface>& location_interfaces);

  /**
   * Add a manipulator to the established platform.
   * This function is OPTIONAL to construct an agent.
   * @param manipulator_frame_interface - interface controlling the end frame
   * @param end_effector_ratio_interface - OPTIONAL - interface controlling the end effector
   * @return - self reference
   */
  NodeFactory& addManipulator(const FrameInterface& manipulator_frame_interface,
                              const RatioInterface& end_effector_ratio_interface);

  /**
   * Add a mission interface.
   * Overwrites any previously added mission interfaces and navigation interfaces
   * @param mission_interface - mission interface
   * @return - self reference
   */
  NodeFactory& addMissionInterface(std::shared_ptr<MissionInterface> mission_interface);

  /**
   * Add a navigation interface.
   * @param navigation_interface - navigation interface
   * @return - self reference
   */
  NodeFactory& addNavigationInterface(const NavigationInterface& navigation_interface);

  /**
   * Add property interfaces
   * @param property_interfaces
   * @return - self reference
   */
  NodeFactory& addProperties(const std::vector<AnyPropertyInterface>& property_interfaces);

  /**
   * Add many confirmed action interfaces
   * @param confirmed_action
   * @return - self reference
   */
  NodeFactory& addConfirmedAction(const ConfirmedActionInterface& confirmed_action);

  /**
   * Add a confirmed action interface
   * @param confirmed_actions
   * @return - self reference
   */
  NodeFactory& addConfirmedActions(const std::vector<ConfirmedActionInterface>& confirmed_actions);

  /**
   * Add touch interface
   * @param touch_interface
   * @return - self reference
   */
  NodeFactory& addTouchInterface(const TouchInterface& touch_interface);

  /**
   * Add a parameter group
   * @param parameter_group
   * @return - self reference
   */
  NodeFactory& addParameterGroup(const ParameterGroupInterface& parameter_group);

  /**
   * Adds several parameter groups
   * @param parameter_groups
   * @return - self reference
   */
  NodeFactory& addParameterGroups(const std::vector<ParameterGroupInterface>& parameter_groups);

private:
  class Implementation;
  std::unique_ptr<Implementation> implementation_;
};

}  // namespace kinesis
