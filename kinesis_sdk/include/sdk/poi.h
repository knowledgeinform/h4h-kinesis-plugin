/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include "sdk/location.h"

#include <string>

namespace kinesis {

/// Describes a POI that can be published using the @ref PoiInterface
struct Poi
{
  /**
   * @brief An identifier that can be used to reference this POI later. This will allow the POI to be edited or deleted.
   *
   * The value is up to you, but it should allow you to uniquely identify the POI's you create. This value is not shared
   * with other users on the network, so it does not need to be globally unique, just unique to your own POI's.
   */
  std::string reference;

  /// The name of the POI
  std::string name;

  /**
   * @brief The icon of the POI to be rendered on the UI
   *
   * Available icons can be found in @ref poi_interface.h.
   */
  std::string icon;

  /// The geographic location of the POI
  kinesis::Location location;

  bool operator==(const Poi& other) const;
  bool operator!=(const Poi& other) const;
};

}  // namespace kinesis
