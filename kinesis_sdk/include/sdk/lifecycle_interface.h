/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

namespace kinesis {
/**
 * An optional interface to manage the lifecycle of owners of kinesis interface implementers
 */
class LifecycleInterface
{
public:
  /**
   * Default Constructor
   */
  LifecycleInterface() = default;

  /**
   * Destructor
   */
  virtual ~LifecycleInterface() = default;

  /**
   * Optional function to be called once when the Kinesis Node is initialized
   */
  virtual void initialize();

  /**
   * Optional function to be called when the Kinesis Node is being shutdown
   */
  virtual void shutdown();
};
}  // namespace kinesis
