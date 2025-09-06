#include <catch2/catch_all.hpp>

#include "../../cpp/cad/adapters/cad-model-reader/fake/FakeCadModelReaderAdapter.hpp"
#include "../../cpp/cad/adapters/logger/fake/FakeLoggerAdapter.hpp"
#include "../../cpp/cad/adapters/model-data-source/fake/FakeModelDataSourceAdapter.hpp"
#include "../../cpp/cad/core/usecase/ListModelPartsUseCase.hpp"

TEST_CASE("ListModelPartsUseCase lists nested assemblies and parts "
          "deterministically") {
  cad::adapters::fake::FakeModelDataSourceAdapter source;
  cad::adapters::fake::FakeCadModelReaderAdapter reader;
  cad::adapters::fake::FakeLoggerAdapter logger;

  std::string locator = "mem:test";
  std::string content = "Assembly: B\nPart: B2\nPart: B1\nEndAssembly\n"
                        "Assembly: A\nPart: A2\nPart: A1\nEndAssembly\n";
  source.registerContent(locator, content);

  cad::usecase::ListModelPartsUseCase usecase(source, reader, logger);
  auto lines = usecase.list(locator);

  std::vector<std::string> expected = {
      "Assembly: Root", "  Assembly: A", "    Part: A1", "    Part: A2",
      "  Assembly: B",  "    Part: B1",  "    Part: B2",
  };

  REQUIRE(lines == expected);
}

TEST_CASE("ListModelPartsUseCase handles missing locator") {
  cad::adapters::fake::FakeModelDataSourceAdapter source;
  cad::adapters::fake::FakeCadModelReaderAdapter reader;
  cad::adapters::fake::FakeLoggerAdapter logger;

  cad::usecase::ListModelPartsUseCase usecase(source, reader, logger);
  auto lines = usecase.list("mem:missing");

  REQUIRE(lines.size() == 1);
  REQUIRE(lines[0].find("ERROR") != std::string::npos);
}
