#pragma once

#include "cpp/cad/core/domain/Identifiers.hpp"
#include <string>

namespace cad::domain {

struct Part {
  PartId id;
  std::string name;
};

} // namespace cad::domain
