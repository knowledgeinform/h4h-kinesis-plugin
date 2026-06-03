/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

/** @file log_interface.h
 * Log operations for LibKinesis
 */

#pragma once

#include <cstdio>
#include <string>

namespace kinesis {

/**
 *
 * An interface to define how logs are written to the file system
 *
 * Helpful tips
 *   - Android Plugin implementors can ignore this interface. It is automatically provided via the Kinesis Android SDK
 *   - If you are looking for a simple interface for logging, see "sdk/logger.h"
 */
class LogInterface
{
public:
  /**
   * @brief Plain text strings for each of the enumerated log levels
   * These must follow the same order as the enum LogLevel above
   */
  static const constexpr char* const kLevelStrings[] = { "Verbose", "Debug", "Info", "Warning", "Error" };

  /**
   * @brief The level, or severity, types available for log messages from least
   * (Verbose) to most (Error)
   * Included also are ultra low levels for frequently sent messages
   */
  enum class LogLevel
  {
    kVerbose,
    kDebug,
    kInfo,
    kWarn,
    kError
  };

  /**
   * Converts the provided log level to a string
   * @param level - input log level
   * @return - printable string matching the log level
   */
  static std::string convertLogLevelToString(LogLevel level);

  /**
   * @brief This must be overwritten by child classes for specific OS behaviors
   * @param priority - log level for this message
   * @param tag  - string name of log message sender
   * @param message  - message to output
   */
  virtual void logLocally(LogLevel priority, std::string tag, std::string message) = 0;
};

}  // namespace kinesis
