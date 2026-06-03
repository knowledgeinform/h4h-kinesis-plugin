/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include "string"

namespace kinesis {

/**
 * Base field class that all field types should extend from
 */
class InputField
{
public:
  /**
   * Get the name of this input field
   * @return field name
   */
  virtual std::string getName() const = 0;

  /**
   * Set the name of this input field
   * @param name - field name
   */
  virtual void setName(std::string_view name) = 0;
};

}  // namespace kinesis
