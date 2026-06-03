/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include "event_handler.h"
#include "poi.h"

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace kinesis {

/**
 * @brief A class responsible for publishing POI's and handling POI edit requests from other users on the network
 *
 * Example usage:
 *
 * @code
 * // Step 1: Generate a POI interface
 * poi_interface_ = PoiInterface::create();
 *
 * // Step 2: Add this interface to the node factory
 * factory.addPoiInterface(poi_interface_);
 *
 * // Step 3: You can now add a POI
 * kinesis::Poi poi;
 * poi.reference = "test_poi";
 * poi.name = "Test";
 * poi.icon = kinesis::PoiInterface::Icon::kTarget;
 * poi.location = kinesis::Location(28.45000, -80.64000, 0.0, kinesis::Location::AltitudeReference::kTakeoff);
 * poi_interface_->insert(poi);
 * @endcode
 *
 * Now let's handle a requests to update poi locations
 * @code
 * auto event_update_location = poi_interface_->eventUpdateLocation();
 * event_update_location->setCallback([=](std::string reference, const Location& new_location) {
 *   if (auto poi = poi_interface_->getPoi(reference))
 *   {
 *     // update the location of the poi and re-publish
 *     poi.location = new_location;
 *     poi_interface_->insert(poi);
 *   }
 * });
 * @endcode
 */
class PoiInterface
{
public:
  /**
   * Create a POI interface object
   * @return A shared pointer to a new PoiInterface instance
   */
  static std::shared_ptr<PoiInterface> create();

  /**
   * @brief Add or update a POI. This POI will be published over the network.
   *
   * If a POI with the same reference already exists, it will be updated, else, a new POI will be created.
   *
   * Example usage:
   *
   * @code
   * kinesis::Poi poi;
   * poi.reference = "test_poi";
   * poi.name = "Test";
   * poi.icon = kinesis::PoiInterface::Icon::kTarget;
   * poi.location = kinesis::Location(28.45000, -80.64000, 0.0, kinesis::Location::AltitudeReference::kTakeoff);
   * poi_interface_->insert(poi);
   * @endcode
   *
   * Note: A generic icon will be rendered for a POI if the icon field is left empty.
   */
  virtual void insert(const Poi& poi) = 0;

  /**
   * @brief Delete the POI
   *
   * Removes the POI from this interface and notifies other nodes on the kinesis network that this
   * POI has been deleted
   *
   * @param reference - the reference string that was provided when the poi was added (see @ref insert)
   */
  virtual void remove(std::string reference) = 0;

  /**
   * @brief Update the location of a POI that was added to the interface using the @ref insert method
   *
   * @param reference - The identifier text that was provided at the time the POI was added (see @ref insert)
   * @param location - the location the POI should be moved to
   * @return true if the location was successfully updated
   */
  virtual bool updateLocation(std::string reference, const Location& location) = 0;

  /**
   * @brief Retrieve a published POI
   *
   * Retrieves a POI that was added with the @ref insert method
   *
   * @param reference - the text identifier that was provided to the @ref insert method
   * @return The POI or std::nullopt if no matching POI was found
   */
  virtual std::optional<Poi> getPoi(std::string reference) const = 0;

  /**
   * @brief Returns an event handler for POI update requests
   *
   * To handle these requests, register a callback with the event handler:
   *
   * @code
   * auto event_update_location = poi_interface_->eventUpdateLocation();
   * event_update_location->setCallback([=](std::string reference, const Location& location) {
   *   if (auto poi = poi_interface_->getPoi(reference))
   *   {
   *     // update the location of the POI and re-publish
   *     poi.location = location;
   *     poi_interface_->insert(poi);
   *   }
   * });
   * @endcode
   *
   * If no callback is registered or the event handler is disabled, it is assumed that the POI's location cannot be
   * changed.
   *
   * @see EventHandler for more details.
   *
   * @return A shared pointer to an EventHandler
   */
  virtual std::shared_ptr<EventHandler<bool(std::string reference, const Location& location)>>
  eventUpdateLocation() const = 0;

  /**
   * @brief Supported POI icons
   *
   * These are the icons that can be used when creating POIs
   */
  struct Icon
  {
    static constexpr char kGeneric[] = "generic";
    static constexpr char kLossOfLink[] = "loss_of_link";
    static constexpr char kApproachPoint[] = "approach_point";
    static constexpr char kLandingPoint[] = "landing_point";
    static constexpr char kAntennaLocation[] = "antenna_location";
    static constexpr char kTarget[] = "target";
  };
};
}  // namespace kinesis
