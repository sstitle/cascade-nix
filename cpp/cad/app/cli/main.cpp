#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "../../adapters/fake/FakeCadModelReaderAdapter.hpp"
#include "../../adapters/fake/FakeLoggerAdapter.hpp"
#include "../../adapters/fake/FakeModelDataSourceAdapter.hpp"
#include "../../core/usecase/ListModelPartsUseCase.hpp"
#include "Formatter.hpp"

int main(int argc, char **argv) {
  if (argc < 3) {
    std::cerr << "Usage: cad-cli list <locator>\n";
    return 1;
  }

  std::string command = argv[1];
  std::string locator = argv[2];

  if (command != "list") {
    std::cerr << "Unknown command: " << command << "\n";
    return 1;
  }

  cad::adapters::fake::FakeModelDataSourceAdapter source;
  cad::adapters::fake::FakeCadModelReaderAdapter reader;
  cad::adapters::fake::FakeLoggerAdapter logger;

  // For demo: if the locator starts with "mem:", register inline content
  if (locator.rfind("mem:", 0) == 0) {
    // Simple demo content
    std::string content =
        "Assembly: Engine\nPart: Piston\nPart: Valve\nEndAssembly\nAssembly: "
        "Frame\nPart: Bolt\nEndAssembly\n";
    source.registerContent(locator, content);
  }

  cad::usecase::ListModelPartsUseCase usecase(source, reader, logger);

  auto lines = usecase.list(locator);
  std::cout << cad::app::cli::Formatter::joinLines(lines) << std::endl;
  return 0;
}
