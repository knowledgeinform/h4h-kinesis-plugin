/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

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

/** @class DemoTrackedVehicle
 * Implements the interfaces necessary to create a fake tracked vehicle agent through the Kinesis SDK
 */
class DemoTrackedVehicle : public kinesis::LifecycleInterface
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
  static constexpr char kPlatformName[] = "DemoUGV";

  /**
   * Platform make
   */
  static constexpr char kPlatformMake[] = "Tomahawk";

  /**
   * Platform model
   */
  static constexpr char kPlatformModel[] = "DemoTrackedVehicle";

  /**
   * Constructor
   * @param node_name - name of this vehicle
   * @param initial_location - initial location of this vehicle
   */
  DemoTrackedVehicle(const std::string& node_name, const kinesis::Location& initial_location);

  /**
   * Destructor
   */
  virtual ~DemoTrackedVehicle();

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
  static constexpr kinesis::Logger kLog = kinesis::Logger("DemoTrackedVehicle");

  /**
   * An ID that identifies the developer for this kinesis plugin
   */
  static constexpr char kIntegratorId[] = "DemoIntegrator";

  /**
   * Name of this node
   */
  std::string name_;

  /**
   * Display name for the Demo parameter
   */
  static constexpr char kParameterDisplayName[] = "Has Toggle Parameter";

  /**
   * Linear velocity limit along the X and Y axes
   */
  static constexpr double kVelocityLimitLinearX = 2.0;

  /**
   * Angular velocity limit about the Z axis
   */
  static constexpr double kVelocityLimitAngularZ = 4.0;

  /**
   * Time of last update
   */
  std::chrono::steady_clock::time_point time_update_ = std::chrono::steady_clock::now();

  /**
   * Energy ratio
   */
  double energy_ratio_ = 1.0;

  /**
   * Signal strength ratio
   */
  double signal_ratio_ = 1.0;

  /**
   * Location
   */
  kinesis::Location location_;

  /**
   * Attitude (rotation to the Local-Level frame from the Platform frame)
   */
  kinesis::Quaternion attitude_;

  /**
   * Linear velocity in this frame
   */
  kinesis::Vector linear_velocity_;

  /**
   * Angular velocity in this frame
   */
  kinesis::Vector angular_velocity_;

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
};

}  // namespace kinesis_example
