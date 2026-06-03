/**
 * TOMAHAWK LICENSED SOFTWARE -- SEE LICENSE FILE FOR DETAILS
 */

#pragma once

#include <functional>
#include <string>

namespace kinesis {
/**
 * Touch interface is used to process a user selected point, as an x and y ratio, to third party SDK users.
 */
struct TouchInterface
{
  /**
   * Field for the unique ID of this location.
   * Only required if needed as a reference in other interfaces (e.g. ViewInterface)
   */
  std::string id;

  /**
   * Required: Method to get whether this interface is ready to process user input.
   * @return true if system is ready to process touch events
   */
  std::function<bool()> enabled;

  /**
   * Notifies the system of a single point selected by the user on a video stream. The acceptable ratio values
   * range from 0.0 to 1.0, where (0.0, 0.0) represents the top-left corner of the stream. The X ratio increases
   * towards the right, and the Y ratio increases towards the bottom. The center point corresponds to the ratio
   * (0.5, 0.5) for both X and Y.
   * @param ratio_x The X ratio coordinate of the screen tap, where valid values range from 0.0 to 1.0.
   * @param ratio_y The Y ratio coordinate of the screen tap, where valid values range from 0.0 to 1.0.
   */
  std::function<void(float ratio_x, float ratio_y)> on_tap;
};
}  // namespace kinesis
