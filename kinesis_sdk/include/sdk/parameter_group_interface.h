/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include "sdk/parameter_interface.h"

#include <functional>
#include <string>

namespace kinesis {

/**
 * Class to allow 3rd party usage of Kinesis Parameter Groups.
 */
struct ParameterGroupInterface
{
  /**
   * Required field for the unique name for the group
   */
  std::string group_name_;

  /**
   * Required method to get the parameters of this platform
   * @return ParameterInterface object containing all the parameters
   */
  std::function<ParameterInterface()> get_parameters_;

  /**
   * Required method to handle a requested change to the platform's parameters
   * @param changed_parameters - KinesisParameter object with only the changed parameters
   */
  std::function<void(const ParameterInterface& changed_parameters)> change_parameters_;
};
}  // namespace kinesis
