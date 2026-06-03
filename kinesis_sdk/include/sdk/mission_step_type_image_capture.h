/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include "sdk/mission_step_type.h"

#include <memory>
#include <string>

namespace kinesis {

/**
 * Object that defines a type of mission step to capture an image
 */
class MissionStepTypeImageCapture : public virtual MissionStepType
{
public:
  /**
   * Name of this step type
   */
  static constexpr char kName[] = "Image Capture";

  /**
   * Get the required field for the view
   */
  virtual std::shared_ptr<ChoiceField> getViewField() = 0;
};

}  // namespace kinesis
