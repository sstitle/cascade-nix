#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include <fstream>
#include <vector>

#include "cpp/cad/adapters/cad-model-reader/opencascade/OpenCascadeCadModelReaderAdapter.hpp"

using cad::adapters::opencascade::OpenCascadeCadModelReaderAdapter;
using cad::domain::Model;

TEST_CASE("OpenCascadeCadModelReaderAdapter can handle empty stream", "[opencascade]") {
  OpenCascadeCadModelReaderAdapter adapter;
  std::istringstream emptyStream("");
  
  Model model = adapter.readModelFromStream(emptyStream);
  
  REQUIRE(model.root.name == "Empty STEP file");
  REQUIRE(model.root.parts.empty());
  REQUIRE(model.root.children.empty());
}

TEST_CASE("OpenCascadeCadModelReaderAdapter handles invalid STEP content", "[opencascade]") {
  OpenCascadeCadModelReaderAdapter adapter;
  std::istringstream invalidStream("This is not a valid STEP file content");
  
  Model model = adapter.readModelFromStream(invalidStream);
  
  REQUIRE(model.root.name == "Error reading STEP file");
}

TEST_CASE("OpenCascadeCadModelReaderAdapter processes valid STEP file", "[opencascade]") {
  OpenCascadeCadModelReaderAdapter adapter;
  
  // Try multiple possible paths for the STEP file
  std::ifstream stepFile;
  std::vector<std::string> possiblePaths = {
    "../test-data/ExampleBallValve.step",
    "test-data/ExampleBallValve.step",
    "../../test-data/ExampleBallValve.step"
  };
  
  bool fileFound = false;
  for (const auto& path : possiblePaths) {
    stepFile.open(path);
    if (stepFile.is_open()) {
      fileFound = true;
      break;
    }
    stepFile.clear(); // Clear error flags for next attempt
  }
  
  if (!fileFound) {
    // Skip this test if we can't find the STEP file
    SKIP("STEP test file not found in expected locations");
    return;
  }
  
  Model model = adapter.readModelFromStream(stepFile);
  
  // With the enhanced adapter, we should get real part names and counts
  REQUIRE(model.root.name.find("STEP Model") == 0);
  REQUIRE(!model.root.parts.empty());
  
  // Check that we got actual part names, not generic ones
  bool hasRealNames = false;
  for (const auto& part : model.root.parts) {
    if (part.name != "Shape_1" && part.name != "STEP_Content" && 
        part.name.find("Entity_") != 0) {
      hasRealNames = true;
      break;
    }
  }
  REQUIRE(hasRealNames);
}

TEST_CASE("OpenCascadeCadModelReaderAdapter handles stream exceptions", "[opencascade]") {
  OpenCascadeCadModelReaderAdapter adapter;
  
  // Create a stream with some content but that will have read issues
  std::istringstream stream("some content");
  stream.setstate(std::ios::failbit);
  
  Model model = adapter.readModelFromStream(stream);
  
  // The adapter should handle this gracefully with meaningful error messages
  REQUIRE((model.root.name.find("Exception reading STEP file") == 0 || 
           model.root.name == "Error reading STEP file" ||
           model.root.name == "Empty STEP file"));
}
