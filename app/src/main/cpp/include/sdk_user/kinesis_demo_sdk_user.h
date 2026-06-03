/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include "sdk/location.h"
#include "sdk/logger.h"

namespace kinesis_example {
class DemoFixedWingVehicle;
class DemoHoveringVehicle;
class DemoTrackedVehicle;

class KinesisDemoSdkUser
{
public:
  // Node type constants
  static constexpr int kNodeTypeDemoHovering = 0;
  static constexpr int kNodeTypeDemoTracked = 1;
  static constexpr int kNodeTypeDemoFixedWing = 2;
  static constexpr int kNodeTypeH4H = 3;

  /**
   * The latitude of the north campus
   */
  static constexpr double kTomahawkNorthCampusLatitude = 28.1286;

  /**
   * The longitude of the north campus
   */
  static constexpr double kTomahawkNorthCampusLongitude = -80.629;

  /**
   * Creates a Kinesis node
   * @param type - type of Node to create
   * @param initial_location - initial location
   */
  void startNode(
      int type,
      const kinesis::Location& initial_location = kinesis::Location(kTomahawkNorthCampusLatitude,
                                                                    kTomahawkNorthCampusLongitude,
                                                                    0.0,
                                                                    kinesis::Location::AltitudeReference::kTakeoff));

  void startNode(
      int type,
      std::string pName,
      std::string pType,
      std::string pControlPort,
      std::string pstatusPort,
      std::string pcamURL,
      const kinesis::Location& initial_location = kinesis::Location(kTomahawkNorthCampusLatitude,
                                                                    kTomahawkNorthCampusLongitude,
                                                                    0.0,
                                                                    kinesis::Location::AltitudeReference::kTakeoff));

private:
  static constexpr kinesis::Logger kLog = kinesis::Logger("KinesisDemoSdkUser");

  /**
   * Construction spacing between vehicles (meters)
   */
  static constexpr double kConstructionSpacing = 5;

  /**
   * Hovering Vehicle counter
   */
  int hovering_vehicle_counter_ = 0;

  /**
   * Tracked vehicle counter
   */
  int tracked_vehicle_counter_ = 0;

  /**
   * Fixed Wing Vehicle counter
   */
  int fixed_wing_vehicle_counter_ = 0;

    /**
   * H4H Vehicle counter
   */
    int h4h_vehicle_counter_ = 0;
};

}  // namespace kinesis_example
