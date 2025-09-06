#pragma once

#include <istream>

#include "cpp/cad/core/domain/Model.hpp"
#include "cpp/cad/core/ports/CadModelReaderPort.hpp"

namespace cad::adapters::opencascade {

class OpenCascadeCadModelReaderAdapter : public cad::ports::CadModelReaderPort {
public:
  cad::domain::Model readModelFromStream(std::istream &stream) override;
};

} // namespace cad::adapters::opencascade
