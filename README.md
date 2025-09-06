# CAD Model Processing System

A hexagonal architecture C++ project for processing CAD models with multiple adapter implementations, built with Nix and modern development tools.

<!-- mdformat-toc start --slug=github --no-anchors --maxlevel=6 --minlevel=1 -->

- [Project Name](#project-name)
  - [Getting Started](#getting-started)
    - [Prerequisites](#prerequisites)
    - [Quick Start](#quick-start)
    - [Available Tools](#available-tools)
    - [Task Automation](#task-automation)
    - [Code Formatting](#code-formatting)
    - [Project Structure](#project-structure)

<!-- mdformat-toc end -->

## Getting Started

### Prerequisites

- [Nix](https://nixos.org/download.html) with flakes enabled
- [direnv](https://direnv.net/) for automatic environment loading

### Quick Start

1. Enter the development shell:

   ```bash
   nix develop
   ```

1. Or with direnv installed:

   ```bash
   direnv allow
   ```

### Available Tools

This development environment includes:

- **C++ toolchain**: Clang, CMake, Ninja for building
- **spdlog**: Fast C++ logging library with colored output
- **nlohmann/json**: Modern C++ JSON library
- **Catch2**: Modern C++ testing framework
- **mask**: Task runner for automating common development tasks
- **treefmt**: Multi-language code formatter
- **git**: Version control
- **direnv/nix-direnv**: Automatic environment loading

### Quick Demo

Run the comprehensive demo to see all functionality:

```bash
mask demo
```

This will:
- Build the project and run all tests
- Demonstrate all 4 adapter combinations
- Show JSON file processing
- Test error handling
- Display the new hexagonal architecture

### Task Automation

This project uses [mask](https://github.com/jacobdeichert/mask) for task automation. View available tasks:

```bash
mask --help
```

### Code Formatting

Use the nix formatter which is managed by `treefmt.nix`:

```bash
nix fmt
```

### Project Structure

```
.
├── flake.nix          # Nix flake configuration
├── shell.nix          # Development shell with C++ dependencies
├── CMakeLists.txt     # Root CMake configuration
├── CMakePresets.json  # CMake presets for consistent builds
├── maskfile.md        # Task definitions and automation
├── treefmt.nix        # Code formatter configuration
├── test-data/         # Sample JSON models for testing
├── cpp/cad/           # Main C++ source code
│   ├── core/          # Domain logic and ports (hexagonal architecture)
│   ├── adapters/      # Adapter implementations organized by port type
│   │   ├── logger/           # Logging adapters (fake, spdlog)
│   │   ├── model-data-source/ # Data source adapters (fake, json)
│   │   └── cad-model-reader/  # Model reader adapters (fake, json)
│   └── app/cli/       # Command-line application
├── cpp/test/          # Unit tests organized by adapter type
└── build/             # CMake build output (generated)
```

### CLI Usage

The CLI supports multiple adapter combinations:

```bash
# Default: fake data source + fake logger
./build/cpp/cad/cad_cli list mem:demo

# Fake data source + spdlog logger (with colors and timestamps)
./build/cpp/cad/cad_cli --logger=spdlog list mem:demo

# JSON file + fake logger
./build/cpp/cad/cad_cli --data-source=json list test-data/simple_model.json

# JSON file + spdlog logger (full featured)
./build/cpp/cad/cad_cli --logger=spdlog --data-source=json list test-data/complex_model.json
```

### Architecture

This project demonstrates **hexagonal architecture** (ports and adapters):

- **Domain Core**: Pure business logic with no external dependencies
- **Ports**: Interfaces defining contracts (LoggerPort, ModelDataSourcePort, CadModelReaderPort)
- **Adapters**: Implementations of ports using specific technologies
- **Modularity**: Easy to swap implementations without changing business logic
- **Testability**: Each adapter can be tested independently
