#pragma once

#include <string>
#include <vector>

namespace cad::app::cli {

struct Formatter {
  static std::string joinLines(const std::vector<std::string> &lines) {
    std::string out;
    for (size_t i = 0; i < lines.size(); ++i) {
      out += lines[i];
      if (i + 1 < lines.size()) out += "\n";
    }
    return out;
  }
};

}


