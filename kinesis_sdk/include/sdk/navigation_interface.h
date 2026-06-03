/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include "sdk/location.h"

#include <chrono>
#include <functional>
#include <optional>
#include <string>
#include <vector>

namespace kinesis {

/**
 * Interface for navigation
 */
struct NavigationInterface
{
  /**
   * Navigation type
   */
  enum class Type
  {
    kGoTo,    // go to a location
    kOrbit,   // orbit a location
    kFollow,  // follow a location
    kSearch,  // search an area
    kLandAt,  // land at a location
  };

  /**
   * Navigation interface state
   */
  enum class State
  {
    kDisabled,    // Not able to navigate (there is a problem that cannot be resolved via this interface)
    kStopped,     // Stopped (can be started)
    kNavigating,  // Actively navigating (can be paused or stopped)
    kPaused       // Paused (can be resumed or stopped)
  };

  /**
   * Required list of the types of navigation supported by this interface
   */
  std::vector<Type> supported_types;

  /**
   * Required method to get the state of the navigation interface.
   * @return current state of the navigation interface
   */
  std::function<State()> get_state;

  /**
   * Required method to get the active type of navigation (if Navigating or Paused)
   * @return active type of navigation (if any)
   */
  std::function<std::optional<Type>()> get_active_type;

  /**
   * Required method to get whether navigation can be started.
   * @param type - type of navigation
   * @return true if given type of navigation can be started
   */
  std::function<bool(Type)> is_start_enabled;

  /**
   * Required method to get whether navigation has "arrived" (reached its final
   * location or holding pattern) since the most recent call to start().
   * @return true if arrived
   */
  std::function<bool()> is_arrived;

  /**
   * Required method to start navigating.
   * Valid to call if state is Stopped.
   * If successful, state should change to Navigating.
   * @param type - type of navigation to start
   * @param nav_parameters - JSON formatted string containing the navigation parameters
   * @return true if successful
   */
  std::function<bool(Type type, const std::string& nav_parameters)> start;

  /**
   * Required method to stop navigating.
   * Valid to call if state is Navigating or Paused.
   * If successful, state should change to Stopped.
   * @return true if successful
   */
  std::function<bool()> stop;

  /**
   * Optional method to pause navigating.
   * Valid to call if state is Navigating.
   * If successful, state should change to Paused.
   * @return true if successful
   */
  std::function<bool()> pause;

  /**
   * Optional method to resume navigating.
   * Valid to call if state is Paused.
   * If successful, state should change to Navigating.
   * @return true if successful
   */
  std::function<bool()> resume;

  /**
   * Optional method to update the target location. If implemented, set one of the following
   * `target_update_...` fields to specify when Kinesis should call this method with an update.
   * @param target_location - updated target location
   */
  std::function<void(const Location& target_location)> update_target_location;

  /**
   * Horizontal distance threshold for target updates (meters).
   * Required if `update_target_location` is provided and `target_update_period_max` is not.
   * The `update_target_location` method will be called if/when the target location has
   * moved more than this horizontal distance since the previous time it was called (subject to the
   * update rate in Kinesis).
   */
  std::optional<double> target_update_horizontal_threshold;

  /**
   * Maximum period for target updates.
   * Required if `update_target_location` is provided and `target_update_horizontal_threshold` is not.
   * The `update_target_location` will be called if/when this duration has elapsed since
   * the previous time it was called (subject to the update rate in Kinesis).
   */
  std::optional<std::chrono::steady_clock::duration> target_update_period_max;

  /**
   * Optional minimum period for target updates.
   * The `update_target_location` will be not be called unless this duration has elapsed
   * since the previous time it was called (subject to the update rate in Kinesis).
   */
  std::optional<std::chrono::steady_clock::duration> target_update_period_min;
};

}  // namespace kinesis
