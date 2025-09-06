#pragma once

#include <string>
#include <vector>
#include "Identifiers.hpp"
#include "Part.hpp"

namespace cad::domain {

struct Assembly {
  AssemblyId id;
  std::string name;
  std::vector<Assembly> children;
  std::vector<Part> parts;
};

}


