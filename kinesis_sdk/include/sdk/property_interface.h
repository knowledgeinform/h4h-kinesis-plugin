/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include <functional>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace kinesis {

enum class PropertyIcon
{
  kNone,
  kInfo,
  kLever,
  kRuler,
  kAltitude,
  kCompass,
  kTemperature,
  kBattery,
  kWarning,
  kTimer,
  kUplink,
  kZoom,
  kAirSpeed,
  kWindSpeed,
  kKey,
};

template <class T>
struct PropertyInterface
{
  /**
   * Optional field for the displayed icon.
   */
  PropertyIcon icon;

  /**
   * Required field for the display name of this property.
   * Must be unique.
   */
  std::string name;

  /**
   * Required method to get the property value
   */
  std::function<std::optional<T>()> get_value;

  /**
   * Optional field for displayed units.
   */
  std::string units;
};

class IntPropertyInterface : public PropertyInterface<int>
{
};

class DoublePropertyInterface : public PropertyInterface<double>
{
};

class StringPropertyInterface : public PropertyInterface<std::string>
{
};

using AnyPropertyInterface = std::variant<IntPropertyInterface, DoublePropertyInterface, StringPropertyInterface>;

}  // namespace kinesis
