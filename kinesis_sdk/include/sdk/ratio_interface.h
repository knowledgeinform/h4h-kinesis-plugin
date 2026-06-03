/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include <functional>
#include <string>

namespace kinesis {

/**
 * Interface for a property that can be expressed as a ratio.
 */
struct RatioInterface
{
  /**
   * Required field for the unique name of this interface
   */
  std::string name;

  /**
   * Method to get whether this interface is enabled for control.
   * Required if ratio can be controlled.
   * @return true if ready to respond to commands
   */
  std::function<bool()> get_is_enabled;

  /**
   * Optional method to get the ratio
   * @return double - current ratio
   */
  std::function<double()> get_ratio;

  /**
   * Optional method to set the desired ratio
   * @param ratio - desired ratio [-1.0 to 1.0]
   */
  std::function<void(double)> set_ratio;

  /**
   * Optional method to set the desired rate of change of the ratio
   * @param rate - desired rate [-1.0 to 1.0]
   */
  std::function<void(double)> set_rate;

  /**
   * Optional method to change the desired ratio by a ratio
   * @param change - desired change in the ratio [-1.0 to 1.0]
   */
  std::function<void(double)> change_by;
};
}  // namespace kinesis
