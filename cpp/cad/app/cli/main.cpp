#include <iostream>
#include <memory>
#include <string>

#include "cpp/cad/adapters/cad-model-reader/fake/FakeCadModelReaderAdapter.hpp"
#include "cpp/cad/adapters/cad-model-reader/json/JsonCadModelReaderAdapter.hpp"
#include "cpp/cad/adapters/cad-model-reader/opencascade/OpenCascadeCadModelReaderAdapter.hpp"
#include "cpp/cad/adapters/logger/fake/FakeLoggerAdapter.hpp"
#include "cpp/cad/adapters/model-data-source/fake/FakeModelDataSourceAdapter.hpp"
#include "cpp/cad/adapters/model-data-source/json/JsonModelDataSourceAdapter.hpp"
#include "cpp/cad/adapters/model-data-source/file/FileModelDataSourceAdapter.hpp"
#include "cpp/cad/adapters/logger/spdlog/SpdlogAdapter.hpp"
#include "cpp/cad/core/usecase/ListModelPartsUseCase.hpp"
#include "cpp/cad/app/cli/Formatter.hpp"

int main(int argc, char **argv) {
  if (argc < 3) {
    std::cerr << "Usage: cad-cli [--logger=fake|spdlog] [--data-source=fake|json|opencascade] list <locator>\n";
    return 1;
  }

  // Parse arguments
  std::string loggerType = "fake"; // default
  std::string dataSourceType = "fake"; // default
  int argIndex = 1;
  
  // Parse optional flags
  while (argIndex < argc && std::string(argv[argIndex]).rfind("--", 0) == 0) {
    std::string flag = argv[argIndex];
    if (flag.rfind("--logger=", 0) == 0) {
      loggerType = flag.substr(9); // Remove "--logger=" prefix
    } else if (flag.rfind("--data-source=", 0) == 0) {
      dataSourceType = flag.substr(14); // Remove "--data-source=" prefix
    }
    argIndex++;
  }
  
  if (argc <= argIndex + 1) {
    std::cerr << "Usage: cad-cli [--logger=fake|spdlog] [--data-source=fake|json|opencascade] list <locator>\n";
    return 1;
  }

  std::string command = argv[argIndex];
  std::string locator = argv[argIndex + 1];

  if (command != "list") {
    std::cerr << "Unknown command: " << command << "\n";
    return 1;
  }

  // Create adapters based on type
  std::unique_ptr<cad::ports::ModelDataSourcePort> source;
  std::unique_ptr<cad::ports::CadModelReaderPort> reader;
  std::unique_ptr<cad::ports::LoggerPort> logger;
  
  // Create data source and reader based on type
  if (dataSourceType == "json") {
    source = std::make_unique<cad::adapters::json::JsonModelDataSourceAdapter>();
    reader = std::make_unique<cad::adapters::json::JsonCadModelReaderAdapter>();
  } else if (dataSourceType == "opencascade") {
    source = std::make_unique<cad::adapters::file::FileModelDataSourceAdapter>(); // Use file data source for direct file access
    reader = std::make_unique<cad::adapters::opencascade::OpenCascadeCadModelReaderAdapter>();
  } else {
    source = std::make_unique<cad::adapters::fake::FakeModelDataSourceAdapter>();
    reader = std::make_unique<cad::adapters::fake::FakeCadModelReaderAdapter>();
  }
  
  // Create logger based on type
  if (loggerType == "spdlog") {
    logger = std::make_unique<cad::adapters::spdlog::SpdlogAdapter>();
  } else {
    logger = std::make_unique<cad::adapters::fake::FakeLoggerAdapter>();
  }

  // For demo: if the locator starts with "mem:" and using fake source, register inline content
  if (dataSourceType == "fake" && locator.rfind("mem:", 0) == 0) {
    // Simple demo content
    std::string content =
        "Assembly: Engine\nPart: Piston\nPart: Valve\nEndAssembly\nAssembly: "
        "Frame\nPart: Bolt\nEndAssembly\n";
    static_cast<cad::adapters::fake::FakeModelDataSourceAdapter*>(source.get())->registerContent(locator, content);
  }

  cad::usecase::ListModelPartsUseCase usecase(*source, *reader, *logger);

  auto lines = usecase.list(locator);
  std::cout << cad::app::cli::Formatter::joinLines(lines) << std::endl;
  return 0;
}
