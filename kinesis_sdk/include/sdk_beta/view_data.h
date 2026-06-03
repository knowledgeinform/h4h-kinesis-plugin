/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include "sdk/view_interface.h"

#include <string>

namespace kinesis {

struct ViewData
{
  /// The node this view belongs to
  std::string node_id;

  /// The ID of the view
  std::string view_id;

  /// Human readable name of the view
  std::string name;

  /// URL of view stream
  std::string url;

  /// The ID of the parent frame
  std::string frame_id;

  /// The ID of the POI located at the center of the view
  std::string center_poi_id;

  /// Flag indicating if this view may be cropped
  bool is_crop_allowed;

  bool operator==(const ViewData& other) const;
  bool operator!=(const ViewData& other) const;
};
}  // namespace kinesis
