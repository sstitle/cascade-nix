#pragma once

#include <string>
#include <vector>

#include "../../core/ports/LoggerPort.hpp"

namespace cad::adapters::fake {

class FakeLoggerAdapter : public cad::ports::LoggerPort {
public:
  void log(cad::ports::LogLevel level, const std::string &message) override {
    entries.push_back(message);
  }

  std::vector<std::string> entries;
};

} // namespace cad::adapters::fake
