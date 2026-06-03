/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include "sdk/field_group.h"

#include <memory>
#include <string>
#include <string_view>

namespace kinesis {

/**
 * Object that defines a type of mission step
 */
class MissionStepType
{
public:
  /**
   * Create a new mission step type
   * @param name - display name
   */
  static std::shared_ptr<MissionStepType> create(std::string_view name);

  /**
   * Get the name of this step type
   */
  virtual std::string getName() const = 0;

  /**
   * Get the fields for the parameters of this step type
   */
  virtual std::shared_ptr<FieldGroup> getFields() const = 0;
};

}  // namespace kinesis
