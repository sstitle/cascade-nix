#include "FakeCadModelReaderAdapter.hpp"

#include <sstream>
#include <stack>
#include <string>

using cad::domain::Assembly;
using cad::domain::Model;
using cad::domain::Part;

namespace cad::adapters::fake {

static std::string trim(const std::string &s) {
  size_t start = s.find_first_not_of(" \t\r\n");
  size_t end = s.find_last_not_of(" \t\r\n");
  if (start == std::string::npos) return "";
  return s.substr(start, end - start + 1);
}

Model FakeCadModelReaderAdapter::readModelFromStream(std::istream &stream) {
  std::string line;
  std::stack<Assembly> stack;
  Assembly root;
  root.name = "Root";
  stack.push(root);

  while (std::getline(stream, line)) {
    line = trim(line);
    if (line.rfind("Assembly:", 0) == 0) {
      Assembly child;
      child.name = trim(line.substr(std::string("Assembly:").size()));
      stack.push(child);
    } else if (line.rfind("Part:", 0) == 0) {
      Part p;
      p.name = trim(line.substr(std::string("Part:").size()));
      stack.top().parts.push_back(p);
    } else if (line == "EndAssembly") {
      if (stack.size() >= 2) {
        Assembly finished = stack.top();
        stack.pop();
        stack.top().children.push_back(finished);
      }
    }
  }

  while (stack.size() > 1) {
    Assembly finished = stack.top();
    stack.pop();
    stack.top().children.push_back(finished);
  }

  Model model;
  model.root = stack.top();
  return model;
}

}


