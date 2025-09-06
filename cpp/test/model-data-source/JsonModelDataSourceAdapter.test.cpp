#include <catch2/catch_all.hpp>
#include <filesystem>
#include <fstream>

#include "cpp/cad/adapters/model-data-source/json/JsonModelDataSourceAdapter.hpp"

TEST_CASE("JsonModelDataSourceAdapter opens valid JSON files") {
  cad::adapters::json::JsonModelDataSourceAdapter adapter;
  
  SECTION("Returns null for non-JSON files") {
    auto stream = adapter.open("test.txt");
    REQUIRE(stream == nullptr);
  }
  
  SECTION("Returns null for non-existent files") {
    auto stream = adapter.open("nonexistent.json");
    REQUIRE(stream == nullptr);
  }
  
  SECTION("Opens existing JSON file") {
    // Create a temporary JSON file
    std::string testFile = "temp_test.json";
    {
      std::ofstream file(testFile);
      file << R"({"test": "data"})";
    }
    
    auto stream = adapter.open(testFile);
    REQUIRE(stream != nullptr);
    REQUIRE(stream->good());
    
    // Read content to verify
    std::string content;
    std::getline(*stream, content);
    REQUIRE(content.find("test") != std::string::npos);
    
    // Cleanup
    std::filesystem::remove(testFile);
  }
}
