#include "ListModelPartsUseCase.hpp"

#include <algorithm>
#include <sstream>

using cad::domain::Assembly;
using cad::domain::Model;
using cad::domain::Part;
using cad::ports::LogLevel;

namespace cad::usecase {

ListModelPartsUseCase::ListModelPartsUseCase(cad::ports::ModelDataSourcePort &source,
                                             cad::ports::CadModelReaderPort &reader,
                                             cad::ports::LoggerPort &logger)
    : source_(source), reader_(reader), logger_(logger) {}

static void collectLines(const Assembly &assembly, std::vector<std::string> &out, int depth) {
  std::string indent(static_cast<size_t>(depth) * 2, ' ');
  out.push_back(indent + "Assembly: " + assembly.name);

  std::vector<Part> parts = assembly.parts;
  std::sort(parts.begin(), parts.end(), [](const Part &a, const Part &b) { return a.name < b.name; });
  for (const auto &p : parts) {
    out.push_back(indent + "  Part: " + p.name);
  }

  std::vector<Assembly> children = assembly.children;
  std::sort(children.begin(), children.end(), [](const Assembly &a, const Assembly &b) { return a.name < b.name; });
  for (const auto &child : children) {
    collectLines(child, out, depth + 1);
  }
}

std::vector<std::string> ListModelPartsUseCase::list(const std::string &locator) const {
  logger_.log(LogLevel::Info, std::string("Opening locator: ") + locator);
  auto stream = source_.open(locator);
  if (!stream || !(*stream)) {
    logger_.log(LogLevel::Error, "Failed to open locator: " + locator);
    return {"ERROR: failed to open locator"};
  }

  Model model = reader_.readModelFromStream(*stream);

  std::vector<std::string> lines;
  collectLines(model.root, lines, 0);
  return lines;
}

}


