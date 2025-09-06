#pragma once

#include <string>
#include <vector>

#include "../domain/Model.hpp"
#include "../ports/CadModelReaderPort.hpp"
#include "../ports/LoggerPort.hpp"
#include "../ports/ModelDataSourcePort.hpp"

namespace cad::usecase {

class ListModelPartsUseCase {
public:
  ListModelPartsUseCase(cad::ports::ModelDataSourcePort &source,
                        cad::ports::CadModelReaderPort &reader,
                        cad::ports::LoggerPort &logger);

  std::vector<std::string> list(const std::string &locator) const;

private:
  cad::ports::ModelDataSourcePort &source_;
  cad::ports::CadModelReaderPort &reader_;
  cad::ports::LoggerPort &logger_;
};

} // namespace cad::usecase
