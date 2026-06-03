/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include <functional>
#include <string>

namespace kinesis {

/**
 * @brief The struct represents a single action. Actions are simple capabilities that can be started/stopped. Examples
 * of actions include: Takeoff, Land, E-Stop, Sit, Stand, etc. Actions are typically made available to users
 * as soft or physical buttons.
 *
 * Usage example:
 *
 * @code
 * kinesis::ActionInterface takeoff;
 * takeoff.name = "Takeoff";
 * takeoff.suggested_button = kinesis::ActionInterface::Button::kGamepadLbA;
 * takeoff.is_start_enabled = [this]() { return isTakeoffEnabled(); };
 * takeoff.is_active = [this]() { return isTakeoffActive(); };
 * takeoff.start = [this]() { return startTakeoff(); };
 * takeoff.stop = [this]() { return stopTakeoff(); };
 * node_factory.addAction(takeoff);
 * @endcode
 */
struct ActionInterface
{
  /**
   * Types of actions. No more than one action should be defined for each non-generic type.
   */
  enum class Type
  {
    /**
     * Default generic action. Actions should be generic unless they match one of the specific types
     * that receive special treatment by kinesis.
     */
    kGeneric,

    /**
     * Action that aborts any autonomous behavior and puts the platform into a basic, ready state (e.g.
     * manual mode with zero throttle).
     *
     * @note This action will be started by pressing the B button (overrides its standard effect)
     */
    kAbort,

    /**
     * Action that lands vertically.
     */
    kVerticalLand,

    /**
     * Action that stops the vehicle in an emergency, possibly damaging it in the process (e.g. shut off
     * propellers).
     *
     * @note This action will be started by holding the B button and accepting by pressing A
     */
    kHardEStop,

    /**
     * Action that returns the vehicle to its home location.
     *
     * @note If this is used, the home location should also be defined via a LocationInterface with
     * LocationInterface::Type::kHome so that users will be able to see and (if supported) move the home location.
     */
    kReturnHome,

    /**
     * Action that resets the view(s) (e.g. center the gimbal and zoom out).
     *
     * @note This action will be started by pressing the Y button (overrides its standard effect)
     */
    kResetView,

    /**
     * Action that changes the platform mode (i.e. whether other interfaces are enabled and/or anything else that
     * changes how the platform can be controlled).
     *
     * @note This action will be started by pressing the X button (overrides its standard effect)
     */
    kChangeMode
  };

  /// Required field. User-facing name of action
  std::string name;

  /// The type of action. Non-generic action types have a special meaning to Kinesis
  Type type = Type::kGeneric;

  /// Optional field for the suggested button to start this action (see @ref Button for valid options)
  std::string suggested_button;

  /// Options for suggested_button field
  struct Button
  {
    /// Gamepad (e.g. Grip) LB+A button
    static constexpr char kGamepadLbA[] = "gamepad_LB_A";

    /// Gamepad (e.g. Grip) LB+B button
    static constexpr char kGamepadLbB[] = "gamepad_LB_B";

    /// Gamepad (e.g. Grip) LB+X button
    static constexpr char kGamepadLbX[] = "gamepad_LB_X";

    /// Gamepad (e.g. Grip) LB+Y button
    static constexpr char kGamepadLbY[] = "gamepad_LB_Y";

    /// Gamepad (e.g. Grip) LB+RB button
    static constexpr char kGamepadLbRb[] = "gamepad_LB_RB";

    /// Gamepad (e.g. Grip) LB+RT button
    static constexpr char kGamepadLbRt[] = "gamepad_LB_RT";
  };

  /// Optional field for whether this action should be hidden
  bool hidden = false;

  /**
   * @brief Method to get whether this action is ready to start
   *
   * @note Required if start method is implemented.
   *
   * @return true if start method is ready to be called
   */
  std::function<bool()> is_start_enabled;

  /**
   * @brief Method to get whether the action is active (i.e. ready to stop)
   *
   * @note Required if stop method is implemented.
   *
   * @return true if stop method is ready to be called
   */
  std::function<bool()> is_active;

  /// Method to start the action
  std::function<void()> start;

  /// Method to stop the action
  std::function<void()> stop;
};
}  // namespace kinesis
