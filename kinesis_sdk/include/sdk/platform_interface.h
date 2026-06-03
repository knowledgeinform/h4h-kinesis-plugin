/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include "sdk/frame_interface.h"
#include "sdk/location.h"
#include "sdk/parameter_interface.h"

#include <array>
#include <functional>
#include <optional>
#include <string>

namespace kinesis {

/**
 * @brief Represents a physical platform (device) that is attached to a kinesis node. This platform can provide a
 * location, battery level, signal strength, etc. All kinesis nodes are required to have a platform interface.
 */
struct PlatformInterface
{
  /**
   * Required field for the unique name of this platform
   */
  std::string name;

  /**
   * Required field for the make of this platform
   */
  std::string make;

  /**
   * Required field for the model of this platform
   */
  std::string model;

  /**
   * Optional method that will be called at a regular rate (e.g. 20 Hz)
   */
  std::function<void()> update;

  /**
   * Optional method to get the location of this platform
   * @return Location describing the location
   */
  std::function<Location()> get_location;

  /**
   * Optional method to get the ratio of energy remaining
   * @return energy ratio (0 = fully depleted, 1 = maximum)
   */
  std::function<std::optional<double>()> get_energy_ratio;

  /**
   * Optional method to get the seconds of energy remaining
   * @return seconds until energy is fully depleted
   */
  std::function<std::optional<int>()> get_energy_seconds;

  /**
   * Optional method to get the voltage of the energy source
   * @return Voltage
   */
  std::function<std::optional<double>()> get_energy_voltage;

  /**
   * Optional method to get the throttle level
   * @return Throttle ratio in range [-1.0, 1.0] where 0.0 is no throttle, and 1.0 is 100% throttle, and -1.0 is 100%
   * reverse throttle
   */
  std::function<std::optional<double>()> get_throttle_ratio;

  /**
   * Optional method to get the state of a platform
   * @return The text representation of the state of the platform (e.g. Park, Drive, etc.)
   */
  std::function<std::optional<std::string>()> get_state;

  /**
   * Optional method to get the signal ratio of this platform
   * @return signal ratio (0 = minimum, 1 = maximum)
   */
  std::function<std::optional<double>()> get_signal_ratio;

  /**
   * Optional method to get the parameters of this platform
   * @return ParameterInterface object containing all the parameters
   */
  std::function<ParameterInterface()> get_parameters;

  /**
   * Optional method to handle a requested change to the platform's parameters
   * @param changed_parameters - KinesisParameter object with only the changed parameters
   */
  std::function<void(const ParameterInterface& changed_parameters)> change_parameters;

  /**
   * Optional interface for the coordinate frame of this platform
   */
  FrameInterface frame;
};

}  // namespace kinesis
