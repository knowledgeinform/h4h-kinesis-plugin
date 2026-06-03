/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include <functional>
#include <optional>
#include <string>
namespace kinesis {
class ConfirmedActionInterface
{
public:
  /**
   * Enumeration for supported states
   */
  enum class State
  {
    kDisabled,  // Disabled
    kEnabled,
    kArmed,  // ACTION is ARMED
    kActive  // ACTION is Active (FIRING)
  };

  /**
   * Struct of states that can be transitioned to.
   */
  struct AvailableStates
  {
    bool disabled = false;
    bool enabled = false;
    bool armed = false;
    bool active = false;
  };

  /**
   * Required method to get the current State.
   * @return state
   */
  std::function<State()> get_state;

  /**
   * Required method to set the desired state
   * @return true if desired state was accepted, False otherwise
   */
  std::function<bool(State)> set_state;

  /**
   * Required method to set the desired state. If you require Active Confirmation,
   * meaning you want to confirm that the current state is still desired,
   * report the current state as an available state. Kinesis will then regularly call the set_state method confirming
   * the current states.
   * @return true if desired state was accepted, False otherwise
   */
  std::function<AvailableStates()> get_available_states;

  /**
   * Required name of the interface
   */
  std::string name;
};
}  // namespace kinesis
