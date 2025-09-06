#pragma once

#include <istream>
#include <string>

#include "../../core/ports/CadModelReaderPort.hpp"

namespace cad::adapters::fake {

// A trivial reader that expects a simple textual format to construct a Model.
// Format:
//   Assembly: <name>
//   Part: <name>
//   Assembly: <child>
//   Part: <name>
//   EndAssembly
// Nested assemblies are delimited by Assembly/EndAssembly.
class FakeCadModelReaderAdapter : public cad::ports::CadModelReaderPort {
public:
  cad::domain::Model readModelFromStream(std::istream &stream) override;
};

} // namespace cad::adapters::fake
