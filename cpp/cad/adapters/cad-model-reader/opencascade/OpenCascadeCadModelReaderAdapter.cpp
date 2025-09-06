#include "cpp/cad/adapters/cad-model-reader/opencascade/OpenCascadeCadModelReaderAdapter.hpp"

#include <sstream>
#include <memory>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <iterator>
#include <string>

// OpenCASCADE headers - ordered for proper Handle<T> template resolution
#include <Standard_Handle.hxx>
#include <Standard_Integer.hxx>
#include <Standard_CString.hxx>
#include <TCollection_ExtendedString.hxx>
#include <TCollection_AsciiString.hxx>
#include <IFSelect_ReturnStatus.hxx>

// Document and application framework
#include <TDocStd_Document.hxx>
#include <XCAFApp_Application.hxx>

// STEP reading
#include <STEPCAFControl_Reader.hxx>

// XDE framework for shape and name handling
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <TDF_LabelSequence.hxx>
#include <TDF_Label.hxx>
#include <TDataStd_Name.hxx>
#include <TopoDS_Shape.hxx>

using cad::domain::Assembly;
using cad::domain::Model;
using cad::domain::Part;

namespace cad::adapters::opencascade {

// Helper function to extract name from a label
std::string extractNameFromLabel(const TDF_Label& label, const std::string& fallbackName) {
  ::opencascade::handle<TDataStd_Name> nameAttr;
  if (label.FindAttribute(TDataStd_Name::GetID(), nameAttr)) {
    TCollection_ExtendedString extName = nameAttr->Get();
    // Convert to ASCII string (this handles UTF-8 conversion)
    TCollection_AsciiString asciiName(extName);
    return std::string(asciiName.ToCString());
  }
  return fallbackName;
}

// Helper function to process a shape label recursively
void processShapeLabel(const ::opencascade::handle<XCAFDoc_ShapeTool>& shapeTool, 
                      const TDF_Label& label, 
                      Assembly& parentAssembly,
                      int& partCounter) {
  
  std::string baseName = "Entity_" + std::to_string(partCounter++);
  std::string shapeName = extractNameFromLabel(label, baseName);
  
  if (shapeTool->IsAssembly(label)) {
    // Create a new assembly
    Assembly assembly;
    assembly.name = shapeName;
    
    // Get components of this assembly
    TDF_LabelSequence components;
    shapeTool->GetComponents(label, components);
    
    // Process each component recursively
    for (Standard_Integer i = 1; i <= components.Length(); i++) {
      processShapeLabel(shapeTool, components.Value(i), assembly, partCounter);
    }
    
    parentAssembly.children.push_back(assembly);
    
  } else if (shapeTool->IsComponent(label)) {
    // This is a component reference - get the actual shape it refers to
    TDF_Label refLabel;
    if (shapeTool->GetReferredShape(label, refLabel)) {
      processShapeLabel(shapeTool, refLabel, parentAssembly, partCounter);
    } else {
      // Fallback: treat as a part
      Part part;
      part.name = shapeName + " (Component)";
      parentAssembly.parts.push_back(part);
    }
    
  } else if (shapeTool->IsSimpleShape(label)) {
    // This is a simple shape (part)
    Part part;
    part.name = shapeName;
    parentAssembly.parts.push_back(part);
    
  } else {
    // Unknown shape type - treat as part
    Part part;
    part.name = shapeName + " (Unknown)";
    parentAssembly.parts.push_back(part);
  }
}

cad::domain::Model OpenCascadeCadModelReaderAdapter::readModelFromStream(std::istream &stream) {
  Model model;
  model.root.name = "STEP Model";
  
  try {
    // Read all stream data into a string
    std::string content((std::istreambuf_iterator<char>(stream)),
                        std::istreambuf_iterator<char>());
    
    if (content.empty()) {
      model.root.name = "Empty STEP file";
      return model;
    }
    
    // Create a temporary file since OpenCASCADE readers work with files
    std::filesystem::path tempFile = std::filesystem::temp_directory_path() / "temp_step_file.step";
    
    // Try to extract model name from the content itself before writing to temp file
    std::string originalModelName = "STEP Model";
    try {
      // Look for FILE_NAME in the content
      size_t fileNamePos = content.find("FILE_NAME(");
      if (fileNamePos != std::string::npos) {
        size_t nameStart = content.find("/* name */ '", fileNamePos);
        if (nameStart != std::string::npos) {
          nameStart += 12; // Length of "/* name */ '"
          size_t nameEnd = content.find("'", nameStart);
          if (nameEnd != std::string::npos) {
            std::string modelName = content.substr(nameStart, nameEnd - nameStart);
            if (!modelName.empty() && modelName != "Unknown") {
              originalModelName = modelName;
            }
          }
        }
      }
    } catch (...) {
      // If extraction fails, keep default name
    }
    
    // Write content to temporary file
    {
      std::ofstream tempStream(tempFile);
      if (!tempStream) {
        model.root.name = "Error creating temporary file";
        return model;
      }
      tempStream << content;
    }
    
    try {
      // Initialize XDE Application and Document
      ::opencascade::handle<XCAFApp_Application> app = XCAFApp_Application::GetApplication();
      ::opencascade::handle<TDocStd_Document> doc;
      app->NewDocument("MDTV-XCAF", doc);
      
      // Create STEP reader with XDE support
      STEPCAFControl_Reader reader;
      reader.SetNameMode(true);  // Enable name reading
      
      // Read the STEP file
      IFSelect_ReturnStatus status = reader.ReadFile(tempFile.string().c_str());
      
      // Clean up temporary file
      std::filesystem::remove(tempFile);
      
      if (status != IFSelect_RetDone) {
        model.root.name = "Error reading STEP file";
        return model;
      }
      
      // Transfer data to the document
      if (!reader.Transfer(doc)) {
        model.root.name = "Error transferring STEP data";
        return model;
      }
      
      // Get the shape tool for accessing hierarchy
      ::opencascade::handle<XCAFDoc_ShapeTool> shapeTool = XCAFDoc_DocumentTool::ShapeTool(doc->Main());
      if (shapeTool.IsNull()) {
        model.root.name = "Error accessing shape tool";
        return model;
      }
      
      // Get free shapes (top-level shapes)
      TDF_LabelSequence freeShapes;
      shapeTool->GetFreeShapes(freeShapes);
      
      if (freeShapes.Length() == 0) {
        model.root.name = "No shapes found in STEP file";
        return model;
      }
      
      // Count total parts and assemblies for the root name
      int totalParts = 0;
      int totalAssemblies = 0;
      
      // First pass: count entities
      TDF_LabelSequence allShapes;
      shapeTool->GetShapes(allShapes);
      for (Standard_Integer i = 1; i <= allShapes.Length(); i++) {
        TDF_Label label = allShapes.Value(i);
        if (shapeTool->IsAssembly(label)) {
          totalAssemblies++;
        } else if (shapeTool->IsSimpleShape(label) || shapeTool->IsComponent(label)) {
          totalParts++;
        }
      }
      
      // Use the model name extracted from the original content
      std::string rootName = originalModelName;
      if (totalAssemblies > 0 || totalParts > 0) {
        rootName += " (" + std::to_string(totalAssemblies) + " assemblies, " + 
                    std::to_string(totalParts) + " parts)";
      }
      model.root.name = rootName;
      
      // Process each free shape
      int partCounter = 1;
      for (Standard_Integer i = 1; i <= freeShapes.Length(); i++) {
        TDF_Label label = freeShapes.Value(i);
        processShapeLabel(shapeTool, label, model.root, partCounter);
      }
      
      return model;
      
    } catch (const std::exception& e) {
      // Clean up temporary file in case of exception
      std::filesystem::remove(tempFile);
      
      // Fallback to simplified parsing
      model.root.name = "STEP Model (XDE parsing failed, using fallback)";
      Part part;
      part.name = "STEP_Content (" + std::string(e.what()) + ")";
      model.root.parts.push_back(part);
      return model;
    }
    
  } catch (const std::exception& e) {
    // Return model with error message
    model.root.name = "Exception reading STEP file: " + std::string(e.what());
    return model;
  }
}

} // namespace cad::adapters::opencascade
