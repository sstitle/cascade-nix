#pragma once

#include <istream>
#include <memory>
#include <string>

namespace cad::ports {

struct ModelDataSourcePort {
  virtual ~ModelDataSourcePort() = default;
  virtual std::unique_ptr<std::istream> open(const std::string &locator) = 0;
};

} // namespace cad::ports
