/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include "sdk/location.h"

#include <functional>
#include <optional>
#include <sstream>
#include <string>

namespace kinesis {

/**
 * Interface for a location shared with Kinesis
 */
struct LocationInterface
{
public:
  /**
   * Optional field for the type of this location
   */
  Location::Type type = Location::Type::kGeneric;

  /**
   * Field for the unique ID of this location.
   * Only required if needed as a reference in other interfaces (e.g. ViewInterface)
   */
  std::string id;

  /**
   * Optional field for the displayed name of this location.
   */
  std::string name;

  /**
   * Optional field for a description of this location.
   */
  std::string description;

  /**
   * Required method to get the location.
   * If the location is unknown, return a default (invalid) `Location` object.
   * @return location
   */
  std::function<Location()> get_location;

  /**
   * Optional method to set the location.
   * If providing this method, you must also provide `is_set_location_enabled`.
   * @param new_location
   * @return true if accepted
   */
  std::function<bool(const Location&)> set_location;

  /**
   * Method to get whether this location can be set (by `set_location`).
   * Required if `set_location` is provided.
   * @return true if `set_location` is enabled
   */
  std::function<bool()> is_set_location_enabled;
};
}  // namespace kinesis
