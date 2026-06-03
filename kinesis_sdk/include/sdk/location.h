/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include <sstream>
#include <string>

namespace kinesis {

class Location
{
public:
  /**
   * Enumeration of altitude frames of reference
   */
  enum class AltitudeReference
  {
    kMeanSeaLevel,  // above mean sea level
    kTerrain,       // above ground level or approximated terrain level using DTED
    kWgs84,         // height above WGS84 ellipsoid
    kTakeoff,       // height above takeoff
    kUnknown        // unknown or unsupported reference
  };

  /**
   * Enumeration of location types
   */
  enum class Type
  {
    kGeneric,           // generic location (no special significance)
    kHome,              // location where the vehicle will end its "Return Home" action
    kViewCenter,        // location where a view's line-of-sight intersects the ground
    kLossOfLink,        // loss of link orbit rally point
    kApproachPoint,     // approach point
    kLandingPoint,      // landing point
    kAntennaLocation,   // antenna location
    kTarget,            // Target location
    kInternalWaypoint,  // Internal AV Waypoint
    kMPUVehicle,        // Manual position update for vehicle
    kMPUCfov            // Manual position update for CFOV
  };

  /**
   * Invalid latitude or longitude
   */
  static constexpr double kInvalidLatLon = 1000.0;

  /**
   * Construct an invalid location
   */
  Location();

  /**
   * Construct a location
   * @param latitude_deg - latitude (degrees)
   * @param longitude_deg - longitude (degrees)
   * @param altitude_meters - altitude (meters)
   * @param altitude_reference - altitude type
   */
  Location(double latitude_deg, double longitude_deg, double altitude_meters, AltitudeReference altitude_reference);

  /**
   * Destructor
   */
  virtual ~Location();

  /**
   * Returns whether this location is valid
   */
  bool isValid() const;

  /**
   * Equality operator
   * @param other
   * @return
   */
  bool operator==(const Location& other) const;

  /**
   * Inequality operator
   * @param other
   * @return
   */
  bool operator!=(const Location& other) const;

  /**
   * Get a location relative to this one
   * @param offset_east Eastward offset to the relative location (meters)
   * @param offset_north Northward offset to the relative location (meters)
   * @param offset_up upward offset to the relative location (meters)
   * @return Location offset from this one
   */
  Location getRelativeLocation(double offset_east, double offset_north, double offset_up) const;

  /**
   * Get a location relative to this one
   * @param range distance to the relative location (meters)
   * @param bearing angle from North (positive East) to the relative location (radians)
   * @return Location at the range and bearing from this one
   */
  Location getRelativeLocation(double range, double bearing) const;

  /**
   * Get the range (distance on earth's surface) to another location
   * @return range (meters), negative if either Location is invalid
   */
  double getRangeTo(const Location& other_location) const;

  /**
   * Get the bearing (angle from North, positive East) to another location
   * @return bearing (radians), zero if either Location is invalid
   */
  double getBearingTo(const Location& other_location) const;

  /**
   * Get the ratio of meters (on earth's surface) to degrees latitude at this location
   * @return ratio
   */
  double getMetersPerDegLatitude() const;

  /**
   * Get the ratio of meters (on earth's surface) to degrees longitude at this location
   * @return ratio
   */
  double getMetersPerDegLongitude() const;

  /**
   * Latitude in degrees
   */
  double latitude = kInvalidLatLon;

  /**
   * Longitude in degrees
   */
  double longitude = kInvalidLatLon;

  /**
   * Altitude in meters
   */
  double altitude = 0.0;

  /**
   * Accuracy of latitude measurement in meters
   * This is the radius in meters from the provided latitude with a 95% probability that the
   * actual object could be within the defined circle.
   */
  double latitude_accuracy = 0.0;

  /**
   * Accuracy of longitude measurement in meters
   * This is the radius in meters from the provided longitude with a 95% probability that the
   * actual object could be within the defined circle.
   */
  double longitude_accuracy = 0.0;

  /**
   * Accuracy of altitude measurement in meters
   * This is the radius in meters from the provided altitude with a 95% probability that the
   * actual object could be within the defined circle.
   */
  double altitude_accuracy = 0.0;

  /**
   * Altitude type
   */
  AltitudeReference altitude_reference = AltitudeReference::kUnknown;

  /**
   * Support for output stream operator.
   *
   * Example
   *  - kLog.v << "My location = " << location
   *
   * @param os
   * @param location
   * @return a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const Location& location)
  {
    return os << "(lat: " << location.latitude << ", lon: " << location.longitude << ", alt: " << location.altitude
              << ")";
  }

private:
  /**
   * Radians per degree
   */
  static constexpr double kRadPerDeg = 0.01745329251;

  /**
   * Average radius of earth (meters)
   */
  static constexpr double kRadiusOfEarth = 6371e3;
};

}  // namespace kinesis
