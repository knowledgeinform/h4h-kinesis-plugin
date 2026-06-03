/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

/** @file demo_hovering_vehicle.h
 * Provides the implementation of the interfaces for a demo hovering vehicle
 */

#pragma once

#include "sdk/action_interface.h"
#include "sdk/lifecycle_interface.h"
#include "sdk/location.h"
#include "sdk/logger.h"
#include "sdk/node_factory.h"
#include "sdk/parameter_interface.h"
#include "sdk/platform_interface.h"
#include "sdk/quaternion.h"
#include "sdk/vector.h"

#include <array>
#include <chrono>
#include <vector>

namespace kinesis_example {

// Forward Declarations
class DemoGimbal;
class DemoView;

/** @class DemoHoveringVehicle
 * Provides the callbacks and access necessary to create a fake hovering vehicle agent through the Kinesis SDK.
 */
class DemoHoveringVehicle : public kinesis::LifecycleInterface
{
public:
  /**
   * Construct a Kinesis Node for an instance of this vehicle
   * @param name - name of this vehicle
   * @param location - initial location of this vehicle
   * @return true if successful
   */
  static bool constructNode(const std::string& name, const kinesis::Location& location);

  /**
   * Platform name
   */
  static constexpr char kPlatformName[] = "DemoHV";

  /**
   * Platform make
   */
  static constexpr char kPlatformMake[] = "Tomahawk";

  /**
   * Platform model
   */
  static constexpr char kPlatformModel[] = "DemoHoveringVehicle";

  /**
   * Constructor
   * @param node_name - name of this vehicle
   * @param initial_location - initial location of this vehicle
   */
  DemoHoveringVehicle(const std::string& node_name, const kinesis::Location& initial_location);

  /**
   * Destructor
   */
  virtual ~DemoHoveringVehicle();

  /**
   * Initialize
   */
  void initialize() override;

  /**
   * Update
   */
  void update();

  /**
   * Shutdown
   */
  void shutdown() override;

  /**
   * Get location
   * @return location
   */
  kinesis::Location getLocation() const;

  /**
   * Get the ratio of energy remaining
   * @return energy ratio
   */
  double getEnergyRatio() const;

  /**
   * Get signal ratio
   * @return signal ratio
   */
  double getSignalRatio() const;

  /**
   * Get parameters
   * @return ParameterInterface object containing all the parameters
   */
  kinesis::ParameterInterface getParameters() const;

  /**
   * Change parameters
   * @param changed_parameters - KinesisParameter object with only the changed parameters
   */
  void changeParameters(const kinesis::ParameterInterface& changed_parameters);

  /**
   * Get whether control is enabled
   * @return true if ready to accept commands
   */
  bool isControlEnabled() const;

  /**
   * Get angular position in platform frame
   */
  kinesis::Quaternion getAngularPosition();

  /**
   * Get linear velocity in parent frame
   * @return - linear velocity of this frame expressed in parent frame (m/s)
   */
  kinesis::Vector getLinearVelocity() const;

  /**
   * Set linear velocity in parent frame
   * @param - linear velocity in parent frame (m/s)
   */
  void setLinearVelocity(kinesis::Vector velocity);

  /**
   * Set angular velocity in parent frame
   * @param - angular velocity in parent frame (m/s)
   */
  void setAngularVelocity(kinesis::Vector velocity);

  /**
   * Return whether takeoff action is enabled
   * @return true if takeoff action is enabled
   */
  bool isTakeoffEnabled() const;

  /**
   * Return whether takeoff action is active
   * @return true if takeoff action is active
   */
  bool isTakeoffActive() const;

  /**
   * Start takeoff action
   */
  void startTakeoff();

  /**
   * Stop takeoff action
   */
  void stopTakeoff();

  /**
   * Return whether land action is enabled
   * @return true if land action is enabled
   */
  bool isLandEnabled() const;

  /**
   * Return whether land action is active
   * @return true if land action is active
   */
  bool isLandActive() const;

  /**
   * Start land action
   */
  void startLand();

  /**
   * Stop land action
   */
  void stopLand();

  /**
   * Get the platform interface
   * @return platform interface
   */
  kinesis::PlatformInterface getPlatform();

  /**
   * Get a vector of the action interfaces
   * @return vector of action interfaces
   */
  std::vector<kinesis::ActionInterface> getActions();

private:
  static constexpr kinesis::Logger kLog = kinesis::Logger("DemoHoveringVehicle");

  /**
   * An ID that identifies the developer for this kinesis plugin
   */
  static constexpr char kIntegratorId[] = "DemoIntegrator";

  /**
   * Name of this node
   */
  std::string name_;

  /**
   * Time of last update
   */
  std::chrono::steady_clock::time_point time_update_ = std::chrono::steady_clock::now();

  /**
   * Energy ratio
   */
  double energy_ratio_ = 1.0;

  /**
   * Signal ratio
   */
  double signal_ratio_ = 1.0;

  /**
   * Takeoff/Landing speed (m/s)
   */
  double takeoff_landing_speed_ = 1.5;

  /**
   * Takeoff altitude (m)
   */
  double takeoff_altitude_ = 3.0;

  /**
   * Enumeration of the vehicle's states
   */
  enum class VehicleState
  {
    kLanded,
    kTakingOff,
    kFlying,
    kLanding
  };

  /**
   * Vehicle state
   */
  VehicleState vehicle_state_ = VehicleState::kLanded;

  /**
   * Location
   */
  kinesis::Location location_;

  /**
   * Linear velocity in Local-Level frame
   */
  kinesis::Vector linear_velocity_;

  /**
   * Angular velocity in Local-Level frame
   */
  kinesis::Vector angular_velocity_;

  /**
   * Attitude (rotation to the local-level frame from the vehicle frame)
   */
  kinesis::Quaternion attitude_;

  /**
   * Vehicle yaw (rotation about the z-axis from East)
   */
  double yaw_ = 0.0;

  /**
   * Vector of views
   */
  std::vector<std::shared_ptr<DemoView>> views_;

  /**
   * Gimbal for cameras
   */
  std::shared_ptr<DemoGimbal> gimbal_;

  /**
   * Demoing vehicle parameter for boolean parameter type
   */
  bool toggle_parameter_ = true;

  /**
   * Display name for the test parameter
   */
  static constexpr char kParameterDisplayName[] = "Has Toggle Parameter";

  /**
   * Linear velocity limit along the X and Y axes
   */
  static constexpr double kVelocityLimitLinearXY = 10.0;

  /**
   * Linear velocity limit along the Z axis
   */
  static constexpr double kVelocityLimitLinearZ = 4.0;

  /**
   * Angular velocity limit about the Z axis
   */
  static constexpr double kVelocityLimitAngularZ = 2.0;
};

}  // namespace kinesis_example
