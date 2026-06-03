/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include "sdk/location.h"

#include <string>

namespace kinesis {

/**
 *  Struct containing string representation for all possible POI icons
 */
struct PoiIcon
{
  // Kinesis Icons
  static constexpr char kGeneric[] = "generic";
  static constexpr char kAgentHomePoint[] = "home_point";
  static constexpr char kLossOfLink[] = "loss_of_link";
  static constexpr char kApproachPoint[] = "approach_point";
  static constexpr char kLandingPoint[] = "landing_point";
  static constexpr char kAntennaLocation[] = "antenna_location";
  static constexpr char kTarget[] = "target";
  static constexpr char kSpi[] = "sensor_point_of_interest";
  static constexpr char kInternalWaypoint[] = "internal_waypoint";
  static constexpr char kMpuVehicle[] = "mpu_vehicle";
  static constexpr char kMpuCfov[] = "mpu_cfov";
  // TAK Icons
  static constexpr char kTakUser[] = "tak_user";
  static constexpr char kTakUnknown[] = "tak_unknown";
  static constexpr char kTakUnknownOther[] = "tak_unknown_other";
  static constexpr char kTakUnknownGround[] = "tak_unknown_ground";
  static constexpr char kTakUnknownAir[] = "tak_unknown_air";
  static constexpr char kTakFriendOther[] = "tak_friend_other";
  static constexpr char kTakFriendGround[] = "tak_friend_ground";
  static constexpr char kTakFriendAir[] = "tak_friend_air";
  static constexpr char kTakNeutralOther[] = "tak_neutral_other";
  static constexpr char kTakNeutralGround[] = "tak_neutral_ground";
  static constexpr char kTakNeutralAir[] = "tak_neutral_air";
  static constexpr char kTakHostileOther[] = "tak_hostile_other";
  static constexpr char kTakHostileGround[] = "tak_hostile_ground";
  static constexpr char kTakHostileAir[] = "tak_hostile_air";
};

/// All of the possible POI types
struct PoiType
{
  static constexpr char kKinesis[] = "Kinesis";
  static constexpr char kTak[] = "TAK";
  static constexpr char kHome[] = "Home";
  static constexpr char kAgent[] = "Agent";
  static constexpr char kViewCenter[] = "ViewCenter";
  static constexpr char kSplash[] = "Splash";
  static constexpr char kSmokeAndTarget[] = "Fire Support Target";
  static constexpr char kInternalWaypoint[] = "Internal Waypoint";
  static constexpr char kTarget[] = "Target";
  static constexpr char kApproach[] = "Approach Point";
  static constexpr char kLanding[] = "Landing Point";
  static constexpr char kLossOfLink[] = "Loss of Link";
  static constexpr char kAntenna[] = "Antenna Location";
};

/**
 *  Struct to be used by Command API to receive POI info
 */
struct PoiData
{
  /// The ID of the POI
  std::string id;

  /// The node this POI belongs to
  std::string node_id;

  /// User facing name of the POI
  std::string name;

  /**
   * @brief The type of the POI
   *
   * This is meant to serve as a means of filtering and to provide insight into the source of the
   * POI. For example, one could determine which POIs have been generated and sent over by TAK by filtering based on
   * the TAK Type. See @ref PoiType
   */
  std::string type;

  /// The icon to show for this POI. See @ref PoiIcon
  std::string icon;

  /// The location of the POI
  kinesis::Location location;

  bool operator==(const PoiData& other) const;
  bool operator!=(const PoiData& other) const;
};

}  // namespace kinesis
