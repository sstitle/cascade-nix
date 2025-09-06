#pragma once

#include "Identifiers.hpp"
#include "Part.hpp"
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
