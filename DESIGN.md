# Adapter Restructuring and JSON Format Design

## New Adapter Directory Structure

The adapters will be reorganized by port type rather than implementation type:

```
cpp/cad/adapters/
├── logger/
│   ├── fake/
│   │   ├── FakeLoggerAdapter.hpp
│   │   └── FakeLoggerAdapter.cpp
│   └── spdlog/
│       ├── SpdlogAdapter.hpp
│       └── SpdlogAdapter.cpp
├── model-data-source/
│   ├── fake/
│   │   ├── FakeModelDataSourceAdapter.hpp
│   │   └── FakeModelDataSourceAdapter.cpp
│   └── json/
│       ├── JsonModelDataSourceAdapter.hpp
│       └── JsonModelDataSourceAdapter.cpp
└── cad-model-reader/
    └── fake/
        ├── FakeCadModelReaderAdapter.hpp
        └── FakeCadModelReaderAdapter.cpp
```

## JSON File Format Specification

The JSON format will represent a flat structure of assemblies and parts. Each assembly and part will have an ID and name, with parent-child relationships defined explicitly.

### Example JSON Structure:

```json
{
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
    },
    {
      "id": "frame",
      "name": "Frame",
      "parent_id": "root"
    }
  ],
  "parts": [
    {
      "id": "piston",
      "name": "Piston",
      "assembly_id": "engine"
    },
    {
      "id": "valve", 
      "name": "Valve",
      "assembly_id": "engine"
    },
    {
      "id": "bolt",
      "name": "Bolt", 
      "assembly_id": "frame"
    }
  ]
}
```

### Mapping to Domain Model:

- **Root Assembly**: Always has `id: "root"` and `parent_id: null`
- **Child Assemblies**: Reference their parent via `parent_id`
- **Parts**: Reference their containing assembly via `assembly_id`
- **IDs**: String identifiers that must be unique within their type
- **Names**: Human-readable names for display

### Equivalent Text Format:
The above JSON represents the same structure as:
```
Assembly: Engine
Part: Piston
Part: Valve
EndAssembly
Assembly: Frame
Part: Bolt
EndAssembly
```

## Implementation Notes

1. **JsonModelDataSourceAdapter** will read JSON files from the filesystem
2. **File Path Convention**: JSON files will be identified by `.json` extension
3. **Error Handling**: Invalid JSON or missing files will return null stream
4. **Validation**: JSON structure will be validated during parsing
5. **Performance**: JSON parsing will happen once when opening the data source

## Logging Changes

1. **Line-by-Line Logging**: Instead of logging joined strings, each line will be logged individually
2. **Fake Logger Output**: Will write directly to `std::cout` with newlines instead of storing in vector
3. **Granular Debug Info**: Better debugging capability with individual line tracking

## Backwards Compatibility

- All existing port interfaces remain unchanged
- Existing text format continues to work through fake adapters
- CLI maintains existing functionality while adding new JSON support
