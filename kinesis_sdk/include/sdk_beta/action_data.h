/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include "sdk/action_interface.h"

#include <string>

namespace kinesis {
/**
 * This struct encapsulates all the necessary information
 * needed to define and manage an action.
 */
struct ActionData
{
  /// The ID of the action
  std::string action_id;

  /// The node this action belongs to
  std::string node_id;

  /// User facing name of the action.
  std::string name;

  /// The type of the action
  ActionInterface::Type type;

  /**
   * @brief True if this action is ready to start
   *
   * Indicates whether the action can be started at the current moment.
   * This flag can be used to control the visibility of a "start action" button
   * for a user interface.
   *
   * For example, a "Takeoff" action would have `is_start_enabled == true` when the drone is landed
   * and ready to takeoff.
   */
  bool is_start_enabled;

  /**
   * @brief True if the action is active (i.e. ready to stop)
   *
   * Indicates whether the action can be stopped / cancelled at the current moment.
   * This flag can be used to control the visibility of a "stop action" button
   * for a user interface.
   *
   * For example, a "Takeoff" action would have `is_stop_enabled == true` when the drone is actively
   * taking off and the takeoff procedure can be canceled.
   */
  bool is_stop_enabled;

  /**
   * @brief Recommended hardware button to start the action
   *
   * A suggestion for a user interface, indicating which physical button on a controller should be used
   * to start this action
   */
  std::string suggested_button;

  /**
   * @brief True if this action should be hidden from the user interface
   *
   * Indicates a preference for an action to be hidden from a user interface if there is a physical
   * controller available that allows this action to be triggered via a physical button.
   *
   * For example, if there is a "Takeoff" action that provides a button hint of "X", a user interface
   * may choose to hide the "Takeoff" action from the UI whenever a controller with a physical "X"
   * button is connected.
   *
   * This can be safely ignored, but provides an opportunity to reduce clutter in the UI when
   * physical buttons are present that would activate this same action.
   */
  bool is_hidden;

  bool operator==(const ActionData& other) const;
  bool operator!=(const ActionData& other) const;
};

}  // namespace kinesis
