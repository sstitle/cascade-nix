#include "cpp/cad/adapters/cad-model-reader/json/JsonCadModelReaderAdapter.hpp"

#include <map>
#include <nlohmann/json.hpp>

using cad::domain::Assembly;
using cad::domain::Model;
using cad::domain::Part;

namespace cad::adapters::json {

cad::domain::Model JsonCadModelReaderAdapter::readModelFromStream(std::istream &stream) {
  try {
    nlohmann::json j;
    stream >> j;
    
    // Parse assemblies
    std::map<std::string, Assembly> assemblyMap;
    
    if (j.contains("assemblies") && j["assemblies"].is_array()) {
      for (const auto& assemblyJson : j["assemblies"]) {
        if (assemblyJson.contains("id") && assemblyJson.contains("name")) {
          Assembly assembly;
          assembly.name = assemblyJson["name"];
          assemblyMap[assemblyJson["id"]] = assembly;
        }
      }
    }
    
    // Parse parts and assign to assemblies
    if (j.contains("parts") && j["parts"].is_array()) {
      for (const auto& partJson : j["parts"]) {
        if (partJson.contains("id") && partJson.contains("name") && partJson.contains("assembly_id")) {
          Part part;
          part.name = partJson["name"];
          
          std::string assemblyId = partJson["assembly_id"];
          if (assemblyMap.find(assemblyId) != assemblyMap.end()) {
            assemblyMap[assemblyId].parts.push_back(part);
          }
        }
      }
    }
    
    // Build hierarchy - find root and build tree
    Assembly root;
    root.name = "Root"; // Default root name
    std::map<std::string, std::string> parentMap; // child_id -> parent_id
    
    // Build parent mapping
    if (j.contains("assemblies") && j["assemblies"].is_array()) {
      for (const auto& assemblyJson : j["assemblies"]) {
        if (assemblyJson.contains("id")) {
          std::string id = assemblyJson["id"];
          if (assemblyJson.contains("parent_id") && !assemblyJson["parent_id"].is_null()) {
            parentMap[id] = assemblyJson["parent_id"];
          } else {
            // This is the root
            root = assemblyMap[id];
          }
        }
      }
    }
    
    // Build the tree structure
    std::function<void(Assembly&, const std::string&)> buildTree = [&](Assembly& parent, const std::string& parentId) {
      for (const auto& [childId, parentIdFromMap] : parentMap) {
        if (parentIdFromMap == parentId) {
          Assembly child = assemblyMap[childId];
          buildTree(child, childId); // Recursively build children
          parent.children.push_back(child);
        }
      }
    };
    
    buildTree(root, "root");
    
    Model model;
    model.root = root;
    return model;
    
  } catch (const nlohmann::json::exception& e) {
    // Return empty model on parse error
    Model model;
    model.root.name = "Root";
    return model;
  }
}

} // namespace cad::adapters::json
