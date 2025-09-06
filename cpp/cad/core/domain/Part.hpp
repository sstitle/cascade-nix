#pragma once

#include "Identifiers.hpp"
#include <string>

namespace cad::domain {

struct Part {
  PartId id;
  std::string name;
};

} // namespace cad::domain
