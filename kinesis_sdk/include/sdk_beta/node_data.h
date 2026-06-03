/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include <string>

namespace kinesis {

/// The data that describes a node on the kinesis network
struct NodeData
{
  /// Universally unique ID for the node
  std::string node_id;

  /// The name of the node (often callsign or user provided name)
  std::string name;

  /**
   * @brief The brand name or the company name reported by the node
   *
   * For example, a node for controlling an AeroVironment Switchblade 300 would have a make of "AeroVironment"
   */
  std::string make;

  /**
   * @brief The model reported by the node
   *
   * For example, a node for controlling an AeroVironment Switchblade 300 would have a model of "Switchblade 300"
   */
  std::string model;

  bool operator==(const NodeData& other) const;
  bool operator!=(const NodeData& other) const;
};

}  // namespace kinesis
