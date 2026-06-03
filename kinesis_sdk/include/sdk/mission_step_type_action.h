/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include "sdk/mission_step_type.h"

namespace kinesis {

/**
 * Object that defines a type of mission step to perform an action
 */
class MissionStepTypeAction : public virtual MissionStepType
{
public:
  /**
   * Name of this step type
   */
  static constexpr char kName[] = "Action";

  /**
   * Get the required field for the action
   */
  virtual std::shared_ptr<ChoiceField> getActionField() const = 0;
};

}  // namespace kinesis
