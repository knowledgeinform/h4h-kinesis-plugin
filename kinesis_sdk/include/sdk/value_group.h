/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include "sdk/bool_field.h"
#include "sdk/choice_field.h"
#include "sdk/double_field.h"
#include "sdk/int_field.h"
#include "sdk/string_field.h"

#include <optional>
#include <string>

namespace kinesis {

/**
 * Container object for a group of values.
 */
class ValueGroup
{
public:
  /**
   * Create a ValueGroup object
   * @return pointer to a new ValueGroup
   */
  static std::shared_ptr<ValueGroup> create();

  /**
   * Get a boolean value in this group
   * @param id - ID of this value
   * @return value (if available)
   */
  virtual std::optional<bool> getBool(std::string_view id) const = 0;

  /**
   * Get a double value in this group
   * @param id - ID of this value
   * @return value (if available)
   */
  virtual std::optional<double> getDouble(std::string_view id) const = 0;

  /**
   * Get an integer value in this group
   * @param id - ID of this value
   * @return value (if available)
   */
  virtual std::optional<int> getInt(std::string_view id) const = 0;

  /**
   * Get a string value in this group
   * @param id - ID of this value
   * @return value (if available)
   */
  virtual std::optional<std::string> getString(std::string_view id) const = 0;

  /**
   * Get a value in this group
   * @param field - field associated with this value
   * @return value (if available)
   */
  virtual std::optional<bool> get(const std::shared_ptr<BoolField>& field) const = 0;

  /**
   * Get a value in this group
   * @param field - field associated with this value
   * @return value (if available)
   */
  virtual std::optional<std::string> get(const std::shared_ptr<ChoiceField>& field) const = 0;

  /**
   * Get a value in this group
   * @param field - field associated with this value
   * @return value (if available)
   */
  virtual std::optional<double> get(const std::shared_ptr<DoubleField>& field) const = 0;

  /**
   * Get a value in this group
   * @param field - field associated with this value
   * @return value (if available)
   */
  virtual std::optional<int> get(const std::shared_ptr<IntField>& field) const = 0;

  /**
   * Get a value in this group
   * @param field - field associated with this value
   * @return value (if available)
   */
  virtual std::optional<std::string> get(const std::shared_ptr<StringField>& field) const = 0;

  /**
   * Set a value in this group
   * @param field - field associated with this value
   * @param value - new value
   */
  virtual void set(const std::shared_ptr<BoolField>& field, bool value) = 0;

  /**
   * Set a value in this group
   * @param field - field associated with this value
   * @param value - new value
   */
  virtual void set(const std::shared_ptr<ChoiceField>& field, std::string_view value) = 0;

  /**
   * Set a value in this group
   * @param field - field associated with this value
   * @param value - new value
   */
  virtual void set(const std::shared_ptr<DoubleField>& field, double value) = 0;

  /**
   * Set a value in this group
   * @param field - field associated with this value
   * @param value - new value
   */
  virtual void set(const std::shared_ptr<IntField>& field, int value) = 0;

  /**
   * Set a value in this group
   * @param field - field associated with this value
   * @param value - new value
   */
  virtual void set(const std::shared_ptr<StringField>& field, std::string_view value) = 0;

  /**
   * Set a boolean value in this group
   * @param id - ID of this value
   * @param value - new value
   */
  virtual void set(std::string_view id, bool value) = 0;

  /**
   * Set a double value in this group
   * @param id - ID of this value
   * @param value - new value
   */
  virtual void set(std::string_view id, double value) = 0;

  /**
   * Set an integer value in this group
   * @param id - ID of this value
   * @param value - new value
   */
  virtual void set(std::string_view id, int value) = 0;

  /**
   * Set a string value in this group
   * @param id - ID of this value
   * @param value - new value
   */
  virtual void set(std::string_view id, std::string_view value) = 0;
};

}  // namespace kinesis
