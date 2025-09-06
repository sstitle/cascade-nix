#include <iostream>
#include <memory>
#include <string>

#include "cpp/cad/adapters/fake/FakeCadModelReaderAdapter.hpp"
#include "cpp/cad/adapters/fake/FakeLoggerAdapter.hpp"
#include "cpp/cad/adapters/fake/FakeModelDataSourceAdapter.hpp"
#include "cpp/cad/adapters/spdlog/SpdlogAdapter.hpp"
#include "cpp/cad/core/usecase/ListModelPartsUseCase.hpp"
#include "cpp/cad/app/cli/Formatter.hpp"

int main(int argc, char **argv) {
  if (argc < 3) {
    std::cerr << "Usage: cad-cli [--logger=fake|spdlog] list <locator>\n";
    return 1;
  }

  // Parse arguments
  std::string loggerType = "fake"; // default
  int argIndex = 1;
  
  // Check for logger flag
  if (argc > 3 && std::string(argv[1]).rfind("--logger=", 0) == 0) {
    loggerType = std::string(argv[1]).substr(9); // Remove "--logger=" prefix
    argIndex = 2;
  }
  
  if (argc <= argIndex + 1) {
    std::cerr << "Usage: cad-cli [--logger=fake|spdlog] list <locator>\n";
    return 1;
  }

  std::string command = argv[argIndex];
  std::string locator = argv[argIndex + 1];

  if (command != "list") {
    std::cerr << "Unknown command: " << command << "\n";
    return 1;
  }

  cad::adapters::fake::FakeModelDataSourceAdapter source;
  cad::adapters::fake::FakeCadModelReaderAdapter reader;
  
  // Create logger based on type
  std::unique_ptr<cad::ports::LoggerPort> logger;
  if (loggerType == "spdlog") {
    logger = std::make_unique<cad::adapters::spdlog::SpdlogAdapter>();
  } else {
    logger = std::make_unique<cad::adapters::fake::FakeLoggerAdapter>();
  }

  // For demo: if the locator starts with "mem:", register inline content
  if (locator.rfind("mem:", 0) == 0) {
    // Simple demo content
    std::string content =
        "Assembly: Engine\nPart: Piston\nPart: Valve\nEndAssembly\nAssembly: "
        "Frame\nPart: Bolt\nEndAssembly\n";
    source.registerContent(locator, content);
  }

  cad::usecase::ListModelPartsUseCase usecase(source, reader, *logger);

  auto lines = usecase.list(locator);
  std::cout << cad::app::cli::Formatter::joinLines(lines) << std::endl;
  return 0;
}
