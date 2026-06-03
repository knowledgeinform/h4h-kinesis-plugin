/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include <optional>
#include <string>
#include <vector>

namespace kinesis {

/**
 * Helper class to allow 3rd party integration of Kinesis Agents.
 * Integrators use and interact with this class through the Platform Status
 * Interface
 */
class ParameterInterface
{
public:
  /**
   * Constructor
   */
  ParameterInterface() = default;

  /**
   * Constructor
   * @deprecated
   */
  explicit ParameterInterface(const std::string& data);

  /**
   * Destructor
   */
  virtual ~ParameterInterface() = default;

  /**
   * Types of parameters supported
   */
  enum class ParameterType
  {
    kUnknown,
    kBoolean,
    kInteger,
    kDouble,
    kString,
    kOption
  };

  /**
   * Query for the type of parameter represented by the display name
   * @param display_name - parameter ID to query
   * @return Parameter type enumeration
   */
  ParameterType getType(const std::string& display_name) const;

  /**
   * Outputs the entire parameter structure as a string.
   * @return - printable string representation of parameters
   */
  std::string dumpAsString() const;

  /**
   * Retrieves all the display names currently in the parameter list
   * @return - vector of all display names available
   */
  std::vector<std::string> getDisplayNames() const;

  /**
   * Add a boolean parameter
   * @param display_name - name to display on the UI for the parameter, also the
   * ID value.  Must be unique among
   * parameters.
   * @param value - boolean value for the parameter
   */
  void addParameterBool(const std::string& display_name, bool value);

  /**
   * Retrieves the parameter value for the given display name
   * @param display_name - name to display on the UI for the parameter, also the
   * ID value.  Must be unique among
   * parameters.
   * @return optional boolean - std::nullopt if the display name is not found,
   * otherwise the parameter value
   */
  std::optional<bool> getParameterBool(const std::string& display_name) const;

  /**
   * Add an integer parameter
   * @param display_name - name to display on the UI for the parameter, also the
   * ID value.  Must be unique among
   * parameters.
   * @param value - integer value for the parameter
   */
  void addParameterInt(const std::string& display_name, int value);

  /**
   * Retrieves the parameter value for the given display name
   * @param display_name - name to display on the UI for the parameter, also the
   * ID value.  Must be unique among
   * parameters.
   * @return optional integer - std::nullopt if the display name is not found,
   * otherwise the parameter value
   */
  std::optional<int> getParameterInt(const std::string& display_name) const;

  /**
   * Add a double parameter
   * @param display_name - name to display on the UI for the parameter, also the
   * ID value.  Must be unique among
   * parameters.
   * @param value - double value for the parameter
   */
  void addParameterDouble(const std::string& display_name, double value);

  /**
   * Retrieves the parameter value for the given display name
   * @param display_name - name to display on the UI for the parameter, also the
   * ID value.  Must be unique among
   * parameters.
   * @return optional double - std::nullopt if the display name is not found,
   * otherwise the parameter value
   */
  std::optional<double> getParameterDouble(const std::string& display_name) const;

  /**
   * Add a string parameter
   * @param display_name - name to display on the UI for the parameter, also the
   * ID value.  Must be unique among
   * parameters.
   * @param value - string value for the parameter
   */
  void addParameterString(const std::string& display_name, std::string value);

  /**
   * Retrieves the parameter value for the given display name
   * @param display_name  - name to display on the UI for the parameter, also
   * the ID value.  Must be unique among
   * parameters.
   * @return optional string - std::nullopt if the display name is not found,
   * otherwise the parameter value
   */
  std::optional<std::string> getParameterString(const std::string& display_name) const;

  /**
   * Add an option selection parameter. Option parameters allow users to select an option from a list
   * @param display_name - name to display on the UI for the parameter, also the ID value. Must be
   * unique among parameters.
   * @param selected_option - string value for the parameter
   * @param options - The list of values that the user can select from. This may be an empty list if simply reporting
   * what value the user selected
   */
  void addParameterOption(const std::string& display_name,
                          const std::string& selected_option,
                          const std::vector<std::string>& options);

  /**
   * The selected option and list of options for an Option parameter
   */
  struct OptionParameter
  {
    /**
     * The selected option
     */
    std::string selected_option;

    /**
     * The list of all available options
     */
    std::vector<std::string> options;
  };

  /**
   * Retrieves the selected option for the request parameter (option parameters allow users to select
   * an option from a list)
   * @param display_name  - name to display on the UI for the parameter, also the ID value. Must be
   * unique among parameters.
   * @return The OptionParameter describing the parameter or std::nullopt if not found
   */
  std::optional<OptionParameter> getParameterOption(const std::string& display_name) const;

  /**
   * Key for the parameter for the altitude to use when returning home.
   * Kinesis will use this parameter for any return behaviors it creates, so if you implement this parameter,
   * add it as a double with units of meters (e.g. `parameters.addParamterDouble(ParameterInterface::kReturnAltitude,
   * 100)`)
   */
  static constexpr char kReturnAltitudeKey[] = "Return Altitude";

private:
  /**
   * Serialized Parameter values
   */
  std::string parameters_;

  /**
   * Log Tag
   */
  static constexpr char kLogTag[] = "ParameterInterface";
};
}  // namespace kinesis
