#include "cpp/cad/adapters/model-data-source/file/FileModelDataSourceAdapter.hpp"

#include <fstream>

namespace cad::adapters::file {

std::unique_ptr<std::istream> FileModelDataSourceAdapter::open(const std::string &locator) {
  auto fileStream = std::make_unique<std::ifstream>(locator, std::ios::binary);
  
  if (!fileStream->is_open()) {
    return nullptr;
  }
  
  return std::move(fileStream);
}

} // namespace cad::adapters::file
