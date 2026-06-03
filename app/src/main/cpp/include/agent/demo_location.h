/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include "sdk/location_interface.h"
#include "sdk/logger.h"

#include <atomic>
#include <mutex>
#include <string>

namespace kinesis_example {

/** @class DemoLocation
 * Example implementation of the location interface
 */
class DemoLocation
{
public:
  /**
   * Constructor
   * @param name - displayed name
   * @param description - description
   * @param type - type of location
   * @param initial_location - initial location
   * @param support_set - whether this interface supports setting the location
   * @param auto_set - whether this interface automatically updates when set (set this to false if you want
   *                   to manually handle requests to set this location from another thread by calling
   *                   popRequestedLocation() to get the latest request and updateLocation() to accept it)
   */
  DemoLocation(const std::string& name,
               const std::string& description,
               kinesis::Location::Type type,
               const kinesis::Location& initial_location,
               bool support_set = false,
               bool auto_set = true);

  /**
   * Destructor
   */
  virtual ~DemoLocation();

  /**
   * Get the location interface
   * @return location interface
   */
  kinesis::LocationInterface getLocationInterface() const;

  /**
   * Get the location
   * @return Location
   */
  kinesis::Location getLocation();

  /**
   * Set the location (if enabled)
   * @param new_location
   * @return true if accepted
   */
  bool setLocation(const kinesis::Location& new_location);

  /**
   * Update the location
   * @param new_location
   */
  void updateLocation(const kinesis::Location& new_location);

  /**
   * Enable/disable setting of this location (if supported)
   * @param enabled
   */
  void enableSetLocation(bool enabled);

  /**
   * Get the latest requested change to this location (if supported and enabled)
   */
  std::optional<kinesis::Location> popRequestedLocation();

private:
  static constexpr kinesis::Logger kLog = kinesis::Logger("DemoLocation");

  /**
   * Location interface
   */
  kinesis::LocationInterface location_interface_;

  /**
   * Location reported to the user of this interface
   * Protected by reported_location_mutex_
   */
  kinesis::Location reported_location_;

  /**
   * Mutex to protect the reported location
   */
  std::mutex reported_location_mutex_;

  /**
   * Location requested by the user of this interface
   * Protected by requested_location_mutex_
   */
  std::optional<kinesis::Location> requested_location_;

  /**
   * Mutex to protect the requested location
   */
  std::mutex requested_location_mutex_;

  /**
   * Flag indicating whether the setLocation() method is enabled
   */
  std::atomic<bool> set_enabled_ = false;

  /**
   * Flag indicating whether setLocation() will automatically update this location
   */
  std::atomic<bool> auto_set_ = true;
};
}  // namespace kinesis_example
