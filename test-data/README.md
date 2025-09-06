# Test Data

This directory contains sample JSON files for testing the CAD model reader functionality.

## Files

- `test_model.json` - Basic model with Engine and Frame assemblies
- `simple_model.json` - Minimal model with just root assembly and parts
- `complex_model.json` - Complex nested aircraft model with multiple assembly levels
- `LEGOMAN.step` - STEP format CAD file for testing OpenCASCADE adapter

## JSON Format

Each JSON file follows the flat structure format:

```json
{
  "assemblies": [
    {
      "id": "unique_id",
      "name": "Display Name", 
      "parent_id": "parent_id_or_null"
    }
  ],
  "parts": [
    {
      "id": "unique_id",
      "name": "Display Name",
      "assembly_id": "containing_assembly_id"
    }
  ]
}
```

## STEP Format

The STEP file (`LEGOMAN.step`) is a standard CAD format that contains 3D geometric data. This file is used for testing the OpenCASCADE adapter implementation.

## Usage

Use with the CLI:
```bash
# JSON format
./build/cpp/cad/cad_cli --data-source=json list test-data/test_model.json

# STEP format (when OpenCASCADE adapter is implemented)
./build/cpp/cad/cad_cli --data-source=opencascade list test-data/LEGOMAN.step
```
