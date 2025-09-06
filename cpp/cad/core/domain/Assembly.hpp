#pragma once

#include "cpp/cad/core/domain/Identifiers.hpp"
#include "cpp/cad/core/domain/Part.hpp"
#include <string>
#include <vector>

namespace cad::domain {

struct Assembly {
  AssemblyId id;
  std::string name;
  std::vector<Assembly> children;
  std::vector<Part> parts;
};

} // namespace cad::domain
