#pragma once

#include <istream>
#include "../domain/Model.hpp"

namespace cad::ports {

struct CadModelReaderPort {
  virtual ~CadModelReaderPort() = default;
  virtual cad::domain::Model readModelFromStream(std::istream &stream) = 0;
};

}


