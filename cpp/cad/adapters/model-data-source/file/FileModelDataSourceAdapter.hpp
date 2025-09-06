#pragma once

#include <istream>
#include <memory>

#include "cpp/cad/core/ports/ModelDataSourcePort.hpp"

namespace cad::adapters::file {

class FileModelDataSourceAdapter : public cad::ports::ModelDataSourcePort {
public:
  std::unique_ptr<std::istream> open(const std::string &locator) override;
};

} // namespace cad::adapters::file
