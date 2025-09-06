#pragma once

#include <fstream>
#include <memory>
#include <string>

#include "cpp/cad/core/ports/ModelDataSourcePort.hpp"

namespace cad::adapters::json {

class JsonModelDataSourceAdapter : public cad::ports::ModelDataSourcePort {
public:
  std::unique_ptr<std::istream> open(const std::string &locator) override;

private:
  // Store file streams to keep them alive
  std::unique_ptr<std::ifstream> file_stream_;
};

} // namespace cad::adapters::json
