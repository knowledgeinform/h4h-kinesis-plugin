#pragma once

#include "sdk/support/logger_base.h"

#include <sstream>

namespace kinesis {
namespace support {

/**
 * Automatically logs when destructor is called
 */
class LoggerScoped
{
public:
  template <typename T>
  LoggerScoped(const LoggerBase& logger, const T& first_item)
    : logger_base_(logger)
  {
    log_message_ << logger.prefix << first_item;
  };

  template <typename T>
  LoggerScoped& operator<<(const T& item)
  {
    log_message_ << item;
    return *this;
  }

  ~LoggerScoped();

private:
  const LoggerBase& logger_base_;
  std::ostringstream log_message_;
};

template <typename T>
LoggerScoped operator<<(const LoggerBase& logger_base, const T& item)
{
  return LoggerScoped(logger_base, item);
}

}  // namespace support
}  // namespace kinesis
