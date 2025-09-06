#pragma once

#include <string>

namespace cad::ports {

enum class LogLevel { Trace, Debug, Info, Warn, Error };

struct LoggerPort {
  virtual ~LoggerPort() = default;
  virtual void log(LogLevel level, const std::string &message) = 0;
};

}


