#pragma once

#include <iostream>
#include <string>

#include "cpp/cad/core/ports/LoggerPort.hpp"

namespace cad::adapters::fake {

class FakeLoggerAdapter : public cad::ports::LoggerPort {
public:
  void log(cad::ports::LogLevel level, const std::string &message) override {
    std::cout << message << "\n";
  }
};

} // namespace cad::adapters::fake
