/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include "sdk/mission_step_type.h"

namespace kinesis {

/**
 * Object that defines a type of mission step to go to a location
 */
class MissionStepTypeGoTo : public virtual MissionStepType
{
public:
  /**
   * Name of this step type
   */
  static constexpr char kName[] = "Go To";
};

}  // namespace kinesis
