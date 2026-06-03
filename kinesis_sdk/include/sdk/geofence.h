/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include "sdk/location.h"

#include <optional>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace kinesis {
/**
 * @brief A structure representing a 2D geographical location.
 */
struct Coordinates
{
  double latitude;
  double longitude;
};

/**
 * @brief A class representing a geographical area with defined boundaries and specific rules.
 */
class Geofence
{
public:
  /**
   * @brief A structure to hold the boundary values specific to a polygon geofence.
   *
   * Polygon bounds contain vertices which maintain the boundary of the geofence.
   * @note The current implementation assumes the final vertex shares an edge with the first vertex in the vertices
   * vector.
   */
  struct PolygonBounds
  {
    std::vector<Coordinates> vertices;
  };

  /**
   * @brief A structure to hold the boundary values specific to a cylindrical geofence.
   *
   * The cylinder bounds contain one vertex, which serves as the center of the cylinder, and a radius around that
   * vertex, defining the boundary of the geofence.
   */
  struct CylinderBounds
  {
    double radius;
    Coordinates center;
  };

  /**
   * User's desired geofence restriction
   */
  enum class Restriction
  {
    kKeepIn,
    kKeepOut
  };

  /**
   * User's desired state of the geofence
   */
  enum class EnableState
  {
    kDestroyed,  // Geofence has been deleted by the User
    kEnabled,    // Geofence is currently enabled and should be followed
    kDisabled,   // Geofence has been disabled by the user for now
  };

  /**
   * Interface's reported violation status for this geofence
   */
  enum class ViolationState
  {
    kNoViolation,
    kViolation,        // Agent is violating geofence
    kViolationIgnored  // User has chosen to ignore the active violation of this geofence
  };

  /**
   * Constant for unknown values
   */
  static constexpr char kUnknown[] = "unknown";

  /**
   * Create invalid/empty geofence
   */
  Geofence();

  /**
   * Copy a geofence
   * @param geofence
   */
  Geofence(const Geofence& geofence);

  // public getters and setters
  const std::string& getUuid() const;
  void setUuid(const std::string& uuid);

  const std::string& getDisplayName() const;
  void setDisplayName(const std::string& display_name);

  Restriction getRestriction() const;
  void setRestriction(Restriction restriction);

  const std::string& getAction() const;
  void setAction(const std::string& action);

  std::optional<double> getCeiling() const;
  void setCeiling(std::optional<double> ceiling);

  Location::AltitudeReference getAltitudeReference() const;
  void setAltitudeReference(Location::AltitudeReference altitude_reference);

  std::optional<double> getRadius() const;
  void setRadius(double radius);

  /**
   * @brief Sets the violation state of the geofence.
   *
   * This function allows setting the current violation state of the geofence. The violation state may indicate one of
   * the following conditions:
   * - `kNoViolation`: The agent is currently not violating the geofence.
   * - `kViolation`: The agent is currently violating the geofence.
   * - `kViolationIgnored`: The user has chosen to ignore the active violation of this geofence.
   *
   * This function should be used to update the state of the geofence based on the current status of any violations.
   * It can be called when a geofence has been violated, when the violation is being ignored,
   * or when the agent is no longer in violation of the geofence.
   *
   * @param violation_state The new violation state to set for the geofence.
   */
  void setViolationState(ViolationState violation_state);
  ViolationState getViolationState() const;

  /**
   * @brief Sets the enabled state of the geofence.
   *
   * This function allows setting the current enabled state of the geofence. The enabled state represents whether the
   * geofence is active and should be enforced, temporarily disabled, or has been destroyed. The possible states are:
   * - `kEnabled`: The geofence is currently enabled and should be followed.
   * - `kDisabled`: The geofence has been disabled by the user.
   * - `kDestroyed`: The geofence has been deleted by the user.
   *
   * This function should be used to manage the lifecycle of a geofence, for instance:
   * - Enabling a geofence when it should start monitoring an area.
   * - Disabling a geofence temporarily from the UI without deleting it.
   * - Destroying a geofence when it should no longer be monitored and is removed permanently.
   *
   * Note: The UI requires that the geofence report its `kDestroyed` state before it can be removed.
   *
   * @param state The new enabled state to set for the geofence.
   */
  void setEnabledState(EnableState state);
  EnableState getEnabledState() const;

  /**
   * Gets the cylindrical bounds of the geofence, if it is of type CylinderBounds.
   * @return An optional containing the CylinderBounds if the geofence has cylindrical bounds, std::nullopt otherwise.
   */
  std::optional<CylinderBounds> getCylinderBounds() const;

  /**
   * Gets the polygon bounds of the geofence, if it is of type PolygonBounds.
   * @return An optional containing the PolygonBounds if the geofence has polygon bounds, std::nullopt otherwise.
   */
  std::optional<PolygonBounds> getPolygonBounds() const;

  /**
   * Sets the bounds of the geofence.
   * @param bounds A std::variant containing either the cylindrical bounds, the polygon bounds, or a monostate
   * indicating no bounds.
   */
  void setBounds(std::variant<CylinderBounds, PolygonBounds, std::monostate> bounds);

  /**
   * Sets the cylindrical bounds of the geofence.
   * @param bounds A CylinderBounds structure containing the radius and center coordinates of the cylindrical geofence.
   */
  void setBounds(CylinderBounds bounds);

  /**
   * Sets the polygon bounds of the geofence.
   * @param bounds A PolygonBounds structure containing the list of vertices defining the polygon geofence.
   */
  void setBounds(PolygonBounds bounds);

  std::vector<Coordinates> getVertices() const;
  void setVertices(std::vector<Coordinates> vertices);

private:
  /**
   * Unique ID of the geofence
   */
  std::string uuid_;
  /**
   * Owning Node's Display name
   */
  std::string display_name_;
  /**
   * Type of geofence
   */
  Restriction restriction_;
  /**
   * Action to take on violation
   */
  std::string action_;

  /**
   * Maximum altitude in meters
   */
  std::optional<double> ceiling_;

  /**
   * Reference point for the maximum altitude
   */
  Location::AltitudeReference altitude_reference_;

  /**
   * Enabled state of the geofence
   */
  EnableState enabled_state_ = EnableState::kEnabled;

  /**
   * Current violation state
   */
  ViolationState violation_state_ = ViolationState::kNoViolation;

  /**
   * Bound type of the geofence
   */
  std::variant<CylinderBounds, PolygonBounds, std::monostate> bounds_;
};

}  // namespace kinesis
