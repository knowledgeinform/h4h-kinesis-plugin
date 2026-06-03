/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include "sdk/event_handler.h"

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace kinesis {
class Geofence;

/**
 * The GeofenceInterface class provides an abstract interface for managing geofences.
 * This includes inserting, removing, disabling geofences, and retrieving the current geofences.
 * The class also includes event handlers for geofence update and delete requests which can
 * be customized by registering callback functions. These functionalities enable effective
 * management and interaction with geofences within the application.
 */
class GeofenceInterface
{
public:
  /**
   * Geofence interface specific configurations
   *
   * These settings are intended to support UI constraints and customization.
   */
  class Config
  {
  public:
    /**
     * Defines actions that can be displayed to the user in the UI.
     */
    struct ViolationActions
    {
      std::vector<std::string> choices;  // List of action choices available to the user
    };
    ViolationActions violation_action;
    bool supports_cylinders = false;
    bool supports_polygons = false;
  };

  /**
   * Create a geofence interface object
   * @param config for geofence interface
   * @return pointer to new geofence interface
   */
  static std::shared_ptr<GeofenceInterface> create(GeofenceInterface::Config config);

  /**
   * @brief Insert a new geofence into the interface.
   *
   * This method adds the specified geofence to the interface for monitoring and management.
   * Once inserted, the geofence becomes active and subject to the SDK user's functionality,
   * such as triggering violation actions and raising alerts. Handling updates or deletions
   * is the responsibility of the interface.
   *
   * @param geofence The Geofence object to be inserted.
   */
  virtual void insert(Geofence geofence) = 0;

  /**
   * @brief Remove a geofence from the interface.
   *
   * This method removes the geofence identified by the given ID from the interface.
   * The geofence will no longer be managed by the interface, and all related actions
   * and alerts will be discontinued.
   *
   * @param id The unique identifier of the geofence to be removed.
   */
  virtual void remove(std::string id) = 0;

  /**
   * @brief Disable geofence in the interface.
   *
   * This method disables the geofence identified by the given ID.
   * The geofence will not trigger any actions or raise any alerts until it is re-enabled.
   * @note that, as of now, this method sets the geofence to a "destroyed" state due to the absence
   * of enable/disable switches in the UI.
   *
   * @param id The unique identifier of the geofence to disable.
   */
  virtual void disable(std::string id) = 0;

  /**
   * @brief Retrieve the current geofences from the interface.
   *
   * This method provides access to all geofences currently managed by the interface.
   * Each geofence is stored in an unordered map where the key is the unique identifier (string) of the geofence
   * and the value is the Geofence object itself.
   *
   * @return std::unordered_map with key-value pairs of geofence identifiers and their corresponding Geofence objects.
   */
  virtual std::unordered_map<std::string, Geofence> getGeofences() = 0;

  /**
   * @brief Returns an event handler for geofence update requests.
   *
   * As an SDK user, you have full control over the value of this geofence.
   * When an end user or Kinesis would like to modify or create a geofence,
   * an "upload" request is sent to this event handler.
   *
   * To handle these requests, register a callback with the event handler:
   *
   * @code
   * geofence_interface_->eventUpload()->setCallback([this](kinesis::Geofence request) {
   *     log.i << "Kinesis is requesting to modify geofence: " << request.getDisplayName();
   * });
   * @endcode
   *
   * If no callback is registered, Kinesis assumes the geofence is read-only
   * and its value cannot be changed.
   *
   * @see EventHandler documentation for more details.
   *
   * @return A shared pointer to an EventHandler
   */
  virtual std::shared_ptr<EventHandler<void(Geofence)>> eventUpload() = 0;

  /**
   * @brief Returns an event handler for geofence delete requests.
   *
   * When an end user or Kinesis would like to remove the geofence,
   * a "delete" request is sent to this event handler.
   *
   * To handle these requests, register a callback with the event handler:
   *
   * @code
   * geofence_interface_->eventDelete()->setCallback([this](std::string id) {
   *     log.i << "Kinesis is requesting to remove geofence with id: " << id;
   * });
   * @endcode
   *
   * If no callback is registered, Kinesis assumes the geofence is read-only
   * and its value cannot be changed.
   *
   * @see EventHandler documentation for more details.
   *
   * @return A shared pointer to an EventHandler
   */
  virtual std::shared_ptr<EventHandler<void(std::string)>> eventDelete() = 0;
};

}  // namespace kinesis
