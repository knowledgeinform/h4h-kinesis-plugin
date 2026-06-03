/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include "sdk/mission_step_type.h"

namespace kinesis {

/**
 * Object that defines a type of mission step to orbit a location
 */
class MissionStepTypeOrbit : public virtual MissionStepType
{
public:
  /**
   * Name of this step type
   */
  static constexpr char kName[] = "Orbit";

  /**
   * Get the required field for the orbit radius (meters)
   */
  virtual std::shared_ptr<DoubleField> getRadiusField() const = 0;

  /**
   * Get the required field for the direction of orbit (true = clockwise)
   */
  virtual std::shared_ptr<BoolField> getDirectionField() const = 0;

  /**
   * Get the optional field for the number of seconds to orbit until step is complete
   */
  virtual std::shared_ptr<IntField> getOrbitSecondsField() const = 0;

  /**
   * Get the optional field for the number of cycles to orbit until step is complete
   */
  virtual std::shared_ptr<DoubleField> getOrbitCyclesField() const = 0;
};

}  // namespace kinesis
