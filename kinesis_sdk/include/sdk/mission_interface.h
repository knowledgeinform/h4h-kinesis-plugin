/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include "sdk/event_handler.h"
#include "sdk/field_group.h"
#include "sdk/location.h"
#include "sdk/mission_step_type.h"
#include "sdk/mission_step_type_action.h"
#include "sdk/mission_step_type_goto.h"
#include "sdk/mission_step_type_image_capture.h"
#include "sdk/mission_step_type_orbit.h"
#include "sdk/value_group.h"

#include <functional>
#include <memory>
#include <optional>
#include <string>

namespace kinesis {

/**
 * Interface object representing the ability to execute a mission
 */
class MissionInterface
{
public:
  /**
   * Container for a mission step
   */
  struct Step
  {
    /**
     * Constructor
     * @param type_name - name of the type of this step
     */
    Step(const std::string& type_name);

    /**
     * @brief Name of the type of this step
     *
     * The name of a recognized type (e.g. MissionStepTypeGoTo::kName) or a custom type of mission step
     */
    std::string type_name;

    /**
     * Location associated with this step
     */
    std::optional<Location> location;

    /**
     * @brief Values of this step
     *
     * Get or set these values via the fields that define them:
     * @code
     * auto orbit_radius_field = kinesis::MissionStepTypeOrbit::getRadiusField();
     * step.values->set(orbit_radius_field, 42.0);
     * auto orbit_radius = step.values->get(orbit_radius_field);
     * @endcode
     */
    std::shared_ptr<ValueGroup> values;
  };

  /**
   * Container for a mission plan
   */
  struct Plan
  {
    /**
     * Constructor
     * @param name - name of mission plan
     */
    Plan(const std::string& name);

    /**
     * Name of this plan (displayed to user)
     */
    std::string name;

    /**
     * Steps in this plan
     */
    std::vector<Step> steps;

    /**
     * Values of this plan
     */
    std::shared_ptr<ValueGroup> values;

    /**
     * Get the index of the final step in this plan
     * @return index of final step (-1 if no steps)
     */
    int getFinalStepIndex() const;
  };

  /**
   * Index of first step
   */
  static constexpr int kFirstStepIndex = 0;

  /**
   * @brief Create a new mission interface
   * @return new MissionInterface object
   */
  static std::shared_ptr<MissionInterface> create();

  /**
   * @brief Get the action step type
   * @return this interface's definition of a step to perform an action
   */
  virtual std::shared_ptr<MissionStepTypeAction> getStepTypeAction() const = 0;

  /**
   * @brief Get the go-to step type
   * @return this interface's definition of a step to go to a location
   */
  virtual std::shared_ptr<MissionStepTypeGoTo> getStepTypeGoTo() const = 0;

  /**
   * @brief Get the orbit step type
   * @return this interface's definition of a step to orbit a location
   */
  virtual std::shared_ptr<MissionStepTypeOrbit> getStepTypeOrbit() const = 0;

  /**
   * @brief Get whether this interface is enabled (ready to accept commands).
   * @return true if enabled (default: true)
   */
  virtual bool isEnabled() const = 0;

  /**
   * @brief Set whether this interface is enabled.
   *
   * Use this method to indicate whether this interface is ready for its event handler callbacks to be called.
   *
   * @note The interface is "enabled" by default, so this method is only needed if the underlying interface can be
   * completely disabled (e.g. E-stop or loss of permission) and is independent of whether individual request handlers
   * are enabled.
   *
   * @param enabled - whether this interface is enabled
   */
  virtual void setEnabled(bool enabled) = 0;

  /**
   * @brief Get whether this interface is executing the active mission.
   * @return true if executing (default: false)
   */
  virtual bool isExecuting() const = 0;

  /**
   * @brief Set whether this interface is executing the active mission.
   *
   * @note If stopping execution, also clear the active step index (see setStepIndex()).
   *
   * @param executing
   */
  virtual void setExecuting(bool executing) = 0;

  /**
   * @brief Get the active mission plan (if any).
   * @return mission plan (default: std::nullopt)
   */
  virtual std::optional<Plan> getPlan() const = 0;

  /**
   * @brief Set the active mission plan.
   * @param plan - mission plan (std::nullopt for none)
   */
  virtual void setPlan(const std::optional<Plan>& plan) = 0;

  /**
   * @brief Get the index of the active step in the active mission plan (if any)
   * @return step index
   */
  virtual std::optional<int> getStepIndex() const = 0;

  /**
   * @brief Set the index of the active step in the active plan.
   *
   * @note Set this to std::nullopt if stopped (leave it on the active step if paused).
   *
   * @param index - index of active step (std::nullopt for none)
   * @return true if successful (false if there is no active plan or the index is out of range)
   */
  virtual bool setStepIndex(std::optional<int> index) = 0;

  /**
   * @brief Update the location of a step in the active plan.
   *
   * @note For efficiency, use this method instead of setPlan() if the only thing that is changing about the plan is the
   * location of a step.
   *
   * @param step_index - index of the step to update
   * @param location - the new locaiton of the step
   * @return true if successful (false if there is no active plan or the index is out of range)
   */
  virtual bool updateStepLocation(int step_index, const Location& location) = 0;

  /**
   * @brief Get the event handler for a request to start the active plan.
   *
   * If implemented, this event handler's callback should start execution of the active mission plan and return true if
   * successful.
   *
   * @note This event handler only needs to be enabled/disabled if there is a restriction external to this interface
   * that could prevent starting a mission (e.g. vehicle must be flying) that is independent of the more general
   * setEnabled() method in this interface.
   *
   * @return event handler for a request to start the mission
   */
  virtual std::shared_ptr<EventHandler<bool()>> eventStart() const = 0;

  /**
   * @brief Get the event handler for a request to stop the active plan.
   *
   * If implemented, this event handler's callback should stop execution of the active mission plan and return true if
   * successful.
   *
   * @note This handler only needs to be enabled/disabled if there is a restriction external to this interface that
   * could prevent stopping a mission (e.g. vehicle can't stop once flying) that is independent of the more general
   * setEnabled() method in this interface.
   *
   * @return event handler for a request to stop the mission
   */
  virtual std::shared_ptr<EventHandler<bool()>> eventStop() const = 0;

  /**
   * @brief Get the event handler for a request to pause the active plan.
   *
   * If implemented, this event handler's callback should pause execution of the active mission plan and return true if
   * successful.
   *
   * @note This handler only needs to be enabled/disabled if there is a restriction external to this
   * interface that could prevent pausing a mission (e.g. vehicle can't stop once flying) that is
   * independent of the more general setEnabled() method in this interface.
   *
   * @return event handler for a request to pause the mission
   */
  virtual std::shared_ptr<EventHandler<bool()>> eventPause() const = 0;

  /**
   * @brief Get the event handler for a request to skip to a specific step in the active plan.
   *
   * If implemented, this event handler's callback should skip execution of the active mission plan to the step
   * indicated by the step_index and return true if successful.
   *
   * @note This handler only needs to be enabled/disabled if there is a restriction external to this
   * interface that could prevent skipping to a step in the mission that is independent of the more
   * general setEnabled() method in this interface.
   *
   * @return event handler for a request to skip to a step in the mission
   */
  virtual std::shared_ptr<EventHandler<bool(int step_index)>> eventSkipTo() const = 0;

  /**
   * @brief Get the event handler for a request to skip to the next step in the active plan.
   *
   * If implemented, this event handler's callback should skip execution of the active mission plan to the next step and
   * return true if successful.
   *
   * @note This event handler's callback does not need to be implemented if eventSkipTo() is.
   *
   * @note This event handler only needs to be enabled/disabled if there is a restriction external to this interface
   * that could prevent skipping to the next step in the mission that is independent of the more general setEnabled()
   * method in this interface.
   *
   * @return event handler for a request to skip to the next step in the mission
   */
  virtual std::shared_ptr<EventHandler<bool()>> eventSkipToNext() const = 0;

  /**
   * @brief Get the event handler for a request to set the active plan.
   *
   * If implemented, this event handler's callback should analyze the provided plan, prepare the mission system to
   * execute it, and (if accepted) announce the change by calling setPlan() or (if rejected) return the reason why the
   * plan was rejected (e.g. "too many steps").
   *
   * @note This event handler only needs to be enabled/disabled if there is a restriction external to this interface
   * that could prevent setting the mission plan (e.g. if the mission cannot be changed while executing) that is
   * independent of the more general setEnabled() method in this interface.
   *
   * @return event handler for a request to set the mission plan
   */
  virtual std::shared_ptr<EventHandler<std::string(const std::optional<Plan>& new_plan)>> eventSetPlan() const = 0;

  /**
   * @brief Get the event handler for a request to update the location of a step in the active plan.
   *
   * If implemented, this callback should update the location of the step in the active plan indicated by the
   * step_index, and (if accepted) announce the change by calling updateStepLocation() and returning true.
   *
   * @note This event handler only needs to be enabled/disabled if there is a restriction external to this interface
   * that could prevent setting the location of a step in the active plan that is independent of the more general
   * setEnabled() method in this interface.
   *
   * @return event handler for a request to set the mission plan
   */
  virtual std::shared_ptr<EventHandler<bool(int step_index, const Location& new_location)>>
  eventUpdateStepLocation() const = 0;
};

}  // namespace kinesis
