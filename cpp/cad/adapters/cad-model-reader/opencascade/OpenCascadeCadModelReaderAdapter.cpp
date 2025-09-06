#include "cpp/cad/adapters/cad-model-reader/opencascade/OpenCascadeCadModelReaderAdapter.hpp"

#include <sstream>
#include <memory>
#include <fstream>
#include <filesystem>

// OpenCASCADE headers
#include <STEPCAFControl_Reader.hxx>
#include <TDocStd_Document.hxx>
#include <XCAFApp_Application.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <TDF_LabelSequence.hxx>
#include <TDataStd_Name.hxx>
#include <TopoDS_Shape.hxx>
#include <TCollection_AsciiString.hxx>
#include <IFSelect_ReturnStatus.hxx>

using cad::domain::Assembly;
using cad::domain::Model;
using cad::domain::Part;

namespace cad::adapters::opencascade {

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
    
    // Write content to temporary file
    {
      std::ofstream tempStream(tempFile);
      if (!tempStream) {
        model.root.name = "Error creating temporary file";
        return model;
      }
      tempStream << content;
    }
    
    // Create STEP reader (simplified approach)
    STEPCAFControl_Reader reader;
    
    // Read the STEP file
    IFSelect_ReturnStatus status = reader.ReadFile(tempFile.string().c_str());
    
    // Clean up temporary file
    std::filesystem::remove(tempFile);
    
    if (status == IFSelect_RetDone) {
      model.root.name = "STEP Model (Loaded Successfully)";
      
      // For now, add a placeholder part to show it worked
      Part part;
      part.name = "STEP Part";
      model.root.parts.push_back(part);
    } else {
      model.root.name = "Error reading STEP file";
    }
    
    return model;
    
  } catch (const std::exception& e) {
    // Return model with error message
    model.root.name = "Exception reading STEP file";
    return model;
  }
}

} // namespace cad::adapters::opencascade
