#include <catch2/catch_all.hpp>
#include <sstream>

#include "cpp/cad/adapters/cad-model-reader/json/JsonCadModelReaderAdapter.hpp"

TEST_CASE("JsonCadModelReaderAdapter parses JSON model correctly") {
  cad::adapters::json::JsonCadModelReaderAdapter reader;
  
  SECTION("Parses valid JSON with assemblies and parts") {
    std::string jsonContent = R"({
      "assemblies": [
        {
          "id": "root",
          "name": "Root",
          "parent_id": null
        },
        {
          "id": "engine",
          "name": "Engine",
          "parent_id": "root"
        }
      ],
      "parts": [
        {
          "id": "piston",
          "name": "Piston",
          "assembly_id": "engine"
        }
      ]
    })";
    
    std::istringstream stream(jsonContent);
    auto model = reader.readModelFromStream(stream);
    
    REQUIRE(model.root.name == "Root");
    REQUIRE(model.root.children.size() == 1);
    REQUIRE(model.root.children[0].name == "Engine");
    REQUIRE(model.root.children[0].parts.size() == 1);
    REQUIRE(model.root.children[0].parts[0].name == "Piston");
  }
  
  SECTION("Handles invalid JSON gracefully") {
    std::string invalidJson = "{ invalid json }";
    std::istringstream stream(invalidJson);
    
    auto model = reader.readModelFromStream(stream);
    REQUIRE(model.root.name == "Root");
    REQUIRE(model.root.children.empty());
    REQUIRE(model.root.parts.empty());
  }
  
  SECTION("Handles empty JSON") {
    std::string emptyJson = "{}";
    std::istringstream stream(emptyJson);
    
    auto model = reader.readModelFromStream(stream);
    REQUIRE(model.root.name == "Root");
    REQUIRE(model.root.children.empty());
    REQUIRE(model.root.parts.empty());
  }
}
