/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include "sdk/lifecycle_interface.h"
#include "sdk/location.h"
#include "sdk/logger.h"
#include "sdk/node_factory.h"
#include "sdk/platform_interface.h"
#include "sdk/quaternion.h"
#include "sdk/vector.h"

#include <chrono>
#include <vector>

namespace kinesis_example {

// Forward Declarations
class DemoGimbal;
class DemoLocation;
class DemoView;

/** @class DemoFixedWingVehicle
 * Implements the interfaces necessary to create a fake fixed-wing vehicle agent through the Kinesis SDK
 */
class DemoFixedWingVehicle : public kinesis::LifecycleInterface
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
  static constexpr char kPlatformName[] = "DemoUAV";

  /**
   * Platform make
   */
  static constexpr char kPlatformMake[] = "Tomahawk";

  /**
   * Platform model
   */
  static constexpr char kPlatformModel[] = "DemoFixedWingVehicle";

  /**
   * Constructor
   * @param platform_name - platform name
   * @param initial_location - initial location of this vehicle
   */
  DemoFixedWingVehicle(const std::string& platform_name, const kinesis::Location& initial_location);

  /**
   * Destructor
   */
  ~DemoFixedWingVehicle() override;

  /**
   * Get the platform interface for this vehicle
   */
  kinesis::PlatformInterface getPlatformInterface();

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
   * Get angular position
   * @return quaternion that rotates from this to the platform frame
   */
  kinesis::Quaternion getAngularPosition();

  /**
   * Get linear velocity in this frame
   * @return - linear velocity of this frame expressed in itself (m/s)
   */
  kinesis::Vector getLinearVelocity() const;

  /**
   * Set linear velocity in this frame
   * @param - linear velocity in this frame (m/s)
   */
  void setLinearVelocity(kinesis::Vector velocity);

  /**
   * Set angular velocity in this frame
   * @param - angular velocity in this frame (m/s)
   */
  void setAngularVelocity(kinesis::Vector velocity);

private:
  static constexpr kinesis::Logger kLog = kinesis::Logger("DemoFixedWingVehicle");

  /**
   * An ID that identifies the developer for this kinesis plugin
   */
  static constexpr char kIntegratorId[] = "DemoIntegrator";

  /**
   * Name of this node
   */
  std::string name_;

  /**
   * Linear velocity limit along the X and Y axes
   */
  static constexpr double kVelocityLimitLinearX = 30.0;

  /**
   * Angular velocity limit about the Y axis when at full speed
   */
  static constexpr double kVelocityLimitAngularY = 1.0;

  /**
   * Angular velocity limit about the Z axis when at full speed
   */
  static constexpr double kVelocityLimitAngularZ = 1.0;

  /**
   * Limit of vehicle pitch
   */
  static constexpr double kPitchLimit = 0.5;

  /**
   * Pitch leveling gain
   */
  static constexpr double kPitchLevelingGain = 0.5;

  /**
   * Ratio of speed limit at which vehicle stalls
   */
  static constexpr double kStallSpeedRatio = 0.5;

  /**
   * Battery depletion rate (ratio / second)
   */
  static constexpr double kBatteryDepletionRate = 1.0 / 300.0;

  /**
   * Time of last update
   */
  std::chrono::steady_clock::time_point time_update_ = std::chrono::steady_clock::now();

  /**
   * Power level ratio
   */
  double energy_ratio_ = 1.0;

  /**
   * Signal strength ratio
   */
  double signal_ratio_ = 1.0;

  /**
   * Location
   */
  kinesis::Location vehicle_location_;

  /**
   * Attitude (rotation to the local-level frame from the vehicle frame)
   */
  kinesis::Quaternion attitude_;

  /**
   * Vehicle pitch (rotation about vehicle y-axis)
   */
  double pitch_ = 0.0;

  /**
   * Vehicle yaw (rotation about local-level z-axis)
   */
  double yaw_ = 0.0;

  /**
   * Ground altitude (assumed for all locations)
   */
  double ground_altitude_ = 0.0;

  /**
   * Flag indicating the vehicle is flying
   */
  bool is_flying_ = false;

  /**
   * Linear velocity in vehicle frame
   */
  kinesis::Vector linear_velocity_;

  /**
   * Angular velocity in vehicle frame
   */
  kinesis::Vector angular_velocity_;

  /**
   * Vector of views
   */
  std::vector<std::shared_ptr<DemoView>> views_;

  /**
   * Location to land
   */
  std::shared_ptr<DemoLocation> location_land_;

  /**
   * Location to approach landing
   */
  std::shared_ptr<DemoLocation> location_approach_;

  /**
   * Location to loiter
   */
  std::shared_ptr<DemoLocation> location_loiter_;

  /**
   * Location of view center on ground
   */
  std::shared_ptr<DemoLocation> location_view_;

  /**
   * Gimbal for cameras
   */
  std::shared_ptr<DemoGimbal> gimbal_;
};

}  // namespace kinesis_example
