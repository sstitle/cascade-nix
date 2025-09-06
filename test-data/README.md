# Test Data

This directory contains sample JSON files for testing the CAD model reader functionality.

## Files

- `test_model.json` - Basic model with Engine and Frame assemblies
- `simple_model.json` - Minimal model with just root assembly and parts
- `complex_model.json` - Complex nested aircraft model with multiple assembly levels

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

## Usage

Use with the CLI:
```bash
./build/cpp/cad/cad_cli --data-source=json list test-data/test_model.json
```
