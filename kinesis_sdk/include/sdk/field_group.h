/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include "bool_field.h"
#include "choice_field.h"
#include "double_field.h"
#include "int_field.h"
#include "string_field.h"

#include <memory>
#include <optional>
#include <string>

namespace kinesis {

/**
 * Interface object representing a field group associated with an agent node
 */
class FieldGroup
{
public:
  /**
   * Create a FieldGroup object
   * @param name - field group name
   * @return pointer to a new FieldGroup
   */
  static std::shared_ptr<FieldGroup> create(std::string_view name);

  /**
   * Get the field group name
   * @return name string
   */
  virtual std::string getName() const = 0;

  /**
   * Set the field group name
   * @param name - field group name
   */
  virtual void setName(std::string_view name) = 0;

  /**
   * Add a bool field to the field group
   * @param field - bool field
   */
  virtual void add(std::shared_ptr<BoolField> field) = 0;

  /**
   * Add a choice field to the field group
   * @param field - choice field
   */
  virtual void add(std::shared_ptr<ChoiceField> field) = 0;

  /**
   * Add an int field to the field group
   * @param field - int field
   */
  virtual void add(std::shared_ptr<IntField> field) = 0;

  /**
   * Add a double field to the field group
   * @param field - double field
   */
  virtual void add(std::shared_ptr<DoubleField> field) = 0;

  /**
   * Add a string field to the field group
   * @param field - string field
   */
  virtual void add(std::shared_ptr<StringField> field) = 0;
};

}  // namespace kinesis
