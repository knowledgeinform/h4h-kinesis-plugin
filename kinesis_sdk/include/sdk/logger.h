#pragma once

#include "sdk/log_interface.h"
#include "sdk/support/logger_scoped.h"

namespace kinesis {

/**
 * @brief The Kinesis Logging Tool.
 *
 * Example Usage:
 *   ```
 *   #include "sdk/logger.h"
 *
 *   class MyClass {
 *   private:
 *     static constexpr Logger kLog = Logger("MyClass");
 *
 *   public:
 *     void performAction() {
 *         kLog.v << "Verbose log: Action started.";
 *         kLog.d << "Debug log: Detailed debugging information.";
 *         kLog.i << "Info log: Informational message.";
 *         kLog.w << "Warning log: A potential issue detected.";
 *         kLog.e << "Error log: An error has occurred.";
 *     }
 *   };
 *   ```
 *
 * Logs are available in 5 levels:
 *   - Verbose (v)
 *   - Debug (d)
 *   - Info (i)
 *   - Warning (w)
 *   - Error (e)
 *
 * All standard filestream formatting operators are supported allowing for formatted output and styling adjustments:
 *   - std::fixed
 *   - std::setprecision
 *   - etc.
 *
 *
 */
class Logger
{
public:
  /**
   * @brief Constructs a logger with a specific tag.
   *
   * @param tag A tag that will be added to the beginning of all logs. Typically, this will be the
   * name of the class that the log is in.
   */
  constexpr Logger(const char* tag)
    : v(LogInterface::LogLevel::kVerbose, tag)
    , d(LogInterface::LogLevel::kDebug, tag)
    , i(LogInterface::LogLevel::kInfo, tag)
    , w(LogInterface::LogLevel::kWarn, tag)
    , e(LogInterface::LogLevel::kError, tag)
  {
  }

  /**
   * Set a prefix that will be added to each log message
   *
   * Example:
   *   Code:
   *   ```
   *   > Logger log = Logger("MyClassName");
   *   > log.setPrefix("MyVehicleName");
   *   > log << "hello!";
   *   ```
   *
   *   Output:
   *   MyClassName [MyVehicleName] hello!
   *
   * @param prefix
   */
  void setLogPrefix(std::string_view prefix);

  /** Verbose */
  support::LoggerBase v;

  /** Debug */
  support::LoggerBase d;

  /** Info */
  support::LoggerBase i;

  /** Warning */
  support::LoggerBase w;

  /** Error */
  support::LoggerBase e;
};

}  // namespace kinesis
