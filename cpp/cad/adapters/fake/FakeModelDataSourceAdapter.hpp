#pragma once

#include <istream>
#include <map>
#include <memory>
#include <sstream>
#include <string>

#include "cpp/cad/core/ports/ModelDataSourcePort.hpp"

namespace cad::adapters::fake {

class FakeModelDataSourceAdapter : public cad::ports::ModelDataSourcePort {
public:
  // Registers a virtual locator with content string (simulates a file or remote
  // resource)
  void registerContent(const std::string &locator, const std::string &content) {
    store_[locator] = content;
  }

  std::unique_ptr<std::istream> open(const std::string &locator) override {
    auto it = store_.find(locator);
    if (it == store_.end()) {
      return std::unique_ptr<std::istream>();
    }
    return std::make_unique<std::istringstream>(it->second);
  }

private:
  std::map<std::string, std::string> store_;
};

} // namespace cad::adapters::fake
