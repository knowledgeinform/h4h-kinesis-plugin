#pragma once

#include "sdk/log_interface.h"

namespace kinesis {
namespace support {

/**
 * Holds the data needed for logging
 */
struct LoggerBase
{
  template <std::size_t N>
  constexpr void copyCStr(char (&dest)[N], const char* src)
  {
    for (std::size_t i = 0; i < N; ++i)
    {
      dest[i] = src[i];
      // break if we reach a null termination
      if (dest[i] == '\0')
      {
        break;
      }
    }
    // ensure null termination for strings >= max length
    dest[N - 1] = '\0';
  }

  /**
   * Constructor
   *
   * const char* is used instead of string to allow for constexpr
   *
   * @param level
   * @param tag
   */
  constexpr LoggerBase(LogInterface::LogLevel level, const char* tag)
    : level(level)
    , log_tag()
    , prefix()
  {
    // set prefix to empty string (windows is mad if we do this in initialization list of constexpr constructor)
    prefix[0] = '\0';
    copyCStr(log_tag, tag);
  };

  void setLogPrefix(std::string_view raw_prefix)
  {
    std::string log_prefix = "[" + std::string(raw_prefix) + "] ";
    copyCStr(prefix, log_prefix.c_str());
  }

  LogInterface::LogLevel level;
  // Max length for the tag
  static constexpr std::size_t kMaxTagLength = 50;
  // using char[] to support constexpr constructor
  char log_tag[kMaxTagLength];
  static constexpr std::size_t kMaxPrefixLength = 50;
  // using char[] to support constexpr constructor
  char prefix[kMaxPrefixLength];
};

}  // namespace support
}  // namespace kinesis
