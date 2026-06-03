/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include <memory>
#include <string>

namespace kinesis {

class ThreadRegistrationInterface : public std::enable_shared_from_this<ThreadRegistrationInterface>
{
protected:
  /**
   * Constructor for thread registration interface.  Does nothing.
   */
  ThreadRegistrationInterface();

  /**
   * Returns the shared pointer for this object
   * @return  - shared pointer to the thread interface
   */
  inline std::shared_ptr<ThreadRegistrationInterface> getSharedPointer()
  {
    return shared_from_this();
  }

public:
  /**
   * Destructor for a thread interface.  Does nothing.
   */
  virtual ~ThreadRegistrationInterface();

  /**
   * Children must implement the thread registration function
   * @param thread_name - name of thread to register
   */
  virtual void registerThread(std::string thread_name) = 0;

  /**
   * Children must implement the thread unregistration function
   * @param thread_name - name of thread to unregister
   */
  virtual void unregisterThread(std::string thread_name) = 0;
};

}  // namespace kinesis
