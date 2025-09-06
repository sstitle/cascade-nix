#pragma once

#include "cpp/cad/core/domain/Model.hpp"
#include <istream>

namespace cad::ports {

struct CadModelReaderPort {
  virtual ~CadModelReaderPort() = default;
  virtual cad::domain::Model readModelFromStream(std::istream &stream) = 0;
};

} // namespace cad::ports
