/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

/** @file mission_module_interface.h
 * Definition interface for supporting a kinesis mission module
 */

#pragma once

#include <string>

namespace kinesis {

/**
 * Provides the functional interface for creating and exercising a mission
 * module on the paired EUD.
 */
class MissionModuleInterface
{
public:
  /**
   * Constructor used for creating child classes.
   * Used to create any resources needed by the interface itself.
   */
  MissionModuleInterface();

  /**
   * Destructor for object
   */
  virtual ~MissionModuleInterface();

  /**
   * Get the name of this mission module
   * @return std::string - name only used for Display purposes
   */
  virtual std::string getName() = 0;

  /**
   * constant string used to identify interactable buttons
   */
  static constexpr char kComponentTypeButton[] = "button";

  /**
   * constant string used to identify interactable text fields
   */
  static constexpr char kComponentTypeText[] = "text";

  /**
   * constant string used to identify interactable checkbox
   */
  static constexpr char kComponentTypeCheck[] = "check";

  /**
   * constant string used to identify interactable poi selection
   */
  static constexpr char kComponentTypePoiSelection[] = "poi_select";

  /**
   * Mission Modules are defined using a simplistic YAML layout.
   * Valid elements :
   *    button, info, text, check, poi_select, ip_select, path_select, tab_group, selector,
   *    broadcast_action, launch_package, video_select, number
   * The implementer of this interface generates a string composed of YAML layout using the
   * specified tags and YAML formatting.
   * INVALID VALUES: you cannot use '-' and ':' in any text strings or identifiers
   *
   * TAG DESCRIPTIONS:
   * button - interactive element that allows a user to provide "button press/release" actions.
   *          This is presented as a text block with click interaction.
   *        - REQUIRED tag:
   *          * id - text label to uniquely identify this element
   *        - Optionally may contain the following tags:
   *          * label - text label to place in the button
   *          * description - text sub-label for clarification
   * info  - NON-interactive element that allows a user to provide text on screen.  This is presented
   *         as a text block with no interaction, the background is set to match the overall UI
   *         background rather than highlighted as an interactive element.  This can be used
   *         as a header to break up sections of interactive elements, or as a read-only text field.
   *        - REQUIRED tag:
   *          * id - text label to uniquely identify this element
   *        - Optionally may contain the following tags:
   *          * label - text to place in the space
   *          * description - text sub-label for clarification
   * text   - interactive element that allows a user to provide string data field for population
   *        - REQUIRED tag:
   *          * id - text label to uniquely identify this element
   *        - Optionally may contain the following tags:
   *          * label - text to place in the space
   *          * description - text sub-label for clarification
   * check  - interactive element that allows a user to provide "box check/uncheck" actions
   *        - REQUIRED tag:
   *          * id - text label to uniquely identify this element
   *        - Optionally may contain the following tags:
   *          * label - text to place in front of the element
   *          * description - text sub-label for clarification
   * poi_select - interactive element that allows a user to choose from the PoI displayed on screen
   *              PROTOTYPE ELEMENT
   *        - REQUIRED tag:
   *          * id - text label to uniquely identify this element
   *        - Optionally may contain the following tags:
   *          * filter - text string to filter selectable PoI types by.  Kinesis default
   *                     PoI types include {Kinesis, TAK} however some applications can
   *                     create custom PoI types.
   * ip_select - interactive element that allows a user to choose from the available network interfaces on the device
   *             RETURN value for this comes as a IPv4 String format xxx.xxx.xxx.xxx through the processTextField
   *             interface.
   *              PROTOTYPE ELEMENT
   *        - REQUIRED tag:
   *          * id - text label to uniquely identify this element
   * @return string yaml data
   */
  virtual std::string getModuleLayoutYaml() = 0;

  /**
   * This is called when the end user enables the mission module.
   */
  virtual void enable();

  /**
   * This is called when the end user disables the mission module
   */
  virtual void disable();

  /**
   * Gets the current enable state of the mission module
   * @return true if enabled
   * @return false if disabled
   */
  virtual bool getEnableState() = 0;

  /**
   * Enumerated values for the current state of a button element
   */
  enum class ButtonState
  {
    kPressed,
    kReleased
  };

  /**
   * This is called by Kinesis when a button in the provided Mission Module is interacted with
   * @param button_id - ID tag for the button (provided by YAML input)
   * @param button_state - current state of the button
   */
  virtual void processButton(std::string button_id, ButtonState button_state);

  /**
   * Enumerated values for the current state of a checkbox element
   */
  enum class CheckboxState
  {
    kChecked,
    kUnChecked
  };

  /**
   * This is called by kinesis when a checkbox in the provided Mission Module is interacted with
   * @param checkbox_id - ID tag for the checkbox (provided by YAML input)
   * @param checkbox_state - current state of the checkbox
   */
  virtual void processCheckbox(std::string checkbox_id, CheckboxState checkbox_state);

  /**
   * This is called by kinesis when a Text field provided by the Mission Module is updated
   * @param text_id - ID tag for the text field (provided by YAML input)
   * @param text_data - current text data displayed in the UI
   */
  virtual void processTextField(std::string text_id, std::string text_data);

  /**
   *  Provides a data class for passing Point of Interest information to the SDK user
   */
  class PoiData
  {
  public:
    PoiData();
    virtual ~PoiData();
    double latitude = 0.0;
    double longitude = 0.0;
    double altitude = 0.0;
    std::string name = "";
    std::string description = "";
  };

  /**
   * This is called by Kinesis when a PoI is selected by the element.
   * @param selection_id - ID of the PoI selection element
   * @param data - PoI data object with all the public PoI data.
   */
  virtual void processPointOfInterestSelection(std::string selection_id, PoiData data);
};

}  // namespace kinesis
