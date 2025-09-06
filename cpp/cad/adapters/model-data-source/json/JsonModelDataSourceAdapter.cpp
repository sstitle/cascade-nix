#include "cpp/cad/adapters/model-data-source/json/JsonModelDataSourceAdapter.hpp"

#include <filesystem>

namespace cad::adapters::json {

std::unique_ptr<std::istream> JsonModelDataSourceAdapter::open(const std::string &locator) {
  // Check if the locator is a file path ending with .json
  if (locator.size() < 5 || locator.substr(locator.size() - 5) != ".json") {
    return nullptr;
  }
  
  // Check if file exists
  if (!std::filesystem::exists(locator)) {
    return nullptr;
  }
  
  // Create file stream
  file_stream_ = std::make_unique<std::ifstream>(locator);
  if (!file_stream_->is_open() || !file_stream_->good()) {
    return nullptr;
  }
  
  // Return the stream (transfers ownership to caller)
  return std::unique_ptr<std::istream>(file_stream_.release());
}

} // namespace cad::adapters::json
