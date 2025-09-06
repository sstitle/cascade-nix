## Background and Motivation

We want a C++ CAD demo that follows hexagonal (ports and adapters) architecture. The first user-facing capability: a CLI that loads a STEP file and outputs a list/tree of subassemblies and parts contained in the model. The CAD reading/interpretation must be a swappable adapter (e.g., OpenCascade-backed adapter vs. a fake/in-memory adapter), with domain logic independent of any specific CAD kernel. We also want the model source to be swappable (filesystem path now; remote server and in-memory buffer later), the inbound UI to be swappable (CLI now; desktop/web later), and logging to be swappable (local stdout/file now; remote hosted later).


## Key Challenges and Analysis

- CAD parsing independence: The core must not depend on OpenCascade or any CAD library types. Use a pure domain model and define ports (interfaces) for outbound dependencies.
- Model representation: Represent assemblies (with nesting), parts, and identifiers in a stable, library-agnostic structure suitable for traversal and printing.
- Swappable model source: Abstract how model bytes/streams are obtained (filesystem, HTTP/remote, in-memory). Keep the core oblivious to transport.
- Swappable CAD reader: Provide a fake adapter for tests and an OpenCascade adapter for STEP parsing.
- Swappable inbound UI: Start with CLI; later add desktop UI and web server without changing core/use cases.
- Swappable logging: Provide a logging port with local (stdout/file) and remote adapters.
- Build and packaging: Organize CMake targets per hexagon layers and use Nix flake for reproducible builds. Make heavy dependencies (OpenCascade, HTTP) optional so the demo runs everywhere.
- Testability/TDD: Use fake adapters for model source, reader, and logger to drive use-case development. Add golden-style tests for CLI output.


## High-level Task Breakdown

1) Define core domain model and ports
- Success criteria: Header-only domain types for `Model`, `Assembly`, `Part`, and identifiers; outbound ports `CadModelReaderPort` (bytes/stream -> `Model`), `ModelDataSourcePort` (locator -> bytes/stream), and `LoggerPort`. No external includes beyond standard library.

2) Implement primary use case: list subassemblies and parts
- Success criteria: `ListModelPartsUseCase` that uses `ModelDataSourcePort` + `CadModelReaderPort` to obtain and interpret the model, then returns a deterministic structured list/tree. Emits logs via `LoggerPort`.

3) Create inbound CLI adapter
- Success criteria: `main.cpp` parses CLI args (including `--source=file|http|memory` and `--logger=stdout|file|remote`), constructs the use case with selected adapters, and prints a stable output. Running `cad-cli list path/to/file.step` shows the hierarchy.

4) Provide fake/in-memory outbound adapters for tests
- Success criteria: `FakeModelDataSourceAdapter` returns in-memory buffers; `FakeCadModelReaderAdapter` maps those buffers to a domain `Model`; `FakeLoggerAdapter` captures logs. Unit tests verify use case behavior and CLI formatting.

5) Provide concrete outbound adapters (guarded options)
- Success criteria: `FilesystemModelDataSourceAdapter` (ON by default), `HttpModelDataSourceAdapter` (optional), `OpenCascadeCadModelReaderAdapter` (optional), `StdoutLoggerAdapter` and `FileLoggerAdapter` (ON), `RemoteLoggerAdapter` (optional). Build guarded behind options.

6) CMake targets and wiring
- Success criteria: Separate targets per layer: `cad_core` (domain+ports), `cad_usecases` (use case), `adapter_*` libraries, `cad_cli`. Clean dependencies: core depends on none; usecases -> core; adapters -> core; app -> usecases + selected adapters.

7) Tests (unit + CLI golden)
- Success criteria: Tests run without OpenCascade/HTTP installed. Validate traversal and CLI output via fake adapters; golden tests for deterministic output.

8) Nix flake updates (optional in first pass)
- Success criteria: Dev shell provides compiler, cmake, test framework; optionally OpenCascade and HTTP client library. Build succeeds inside flake.


## Proposed Repository Structure (Hexagonal)

```
cpp/
  cad/
    core/
      domain/
        Model.hpp
        Assembly.hpp
        Part.hpp
        Identifiers.hpp
      ports/
        CadModelReaderPort.hpp        # outbound: bytes/stream -> Model
        ModelDataSourcePort.hpp       # outbound: locator -> bytes/stream
        LoggerPort.hpp                # outbound: logging
      usecase/
        ListModelPartsUseCase.hpp
        ListModelPartsUseCase.cpp
    adapters/
      data_source/
        filesystem/
          FilesystemModelDataSourceAdapter.hpp
          FilesystemModelDataSourceAdapter.cpp
        http/
          HttpModelDataSourceAdapter.hpp
          HttpModelDataSourceAdapter.cpp
        memory/
          MemoryModelDataSourceAdapter.hpp
          MemoryModelDataSourceAdapter.cpp
      logger/
        stdout/
          StdoutLoggerAdapter.hpp
          StdoutLoggerAdapter.cpp
        file/
          FileLoggerAdapter.hpp
          FileLoggerAdapter.cpp
        remote/
          RemoteLoggerAdapter.hpp
          RemoteLoggerAdapter.cpp
      fake/
        FakeCadModelReaderAdapter.hpp
        FakeCadModelReaderAdapter.cpp
        FakeModelDataSourceAdapter.hpp
        FakeModelDataSourceAdapter.cpp
        FakeLoggerAdapter.hpp
        FakeLoggerAdapter.cpp
      opencascade/
        OpenCascadeCadModelReaderAdapter.hpp
        OpenCascadeCadModelReaderAdapter.cpp
    app/
      cli/
        main.cpp                      # inbound adapter + composition root
        Formatter.hpp
        Formatter.cpp
      desktop/                        # placeholder for later
      web/                            # placeholder for later
    CMakeLists.txt
  CMakeLists.txt

tests/
  usecase/
    ListModelPartsUseCase.test.cpp
  cli/
    list_command_golden.test.cpp
  adapters/
    fake/
      FakeCadModelReaderAdapter.test.cpp
      FakeModelDataSourceAdapter.test.cpp
      FakeLoggerAdapter.test.cpp

CMakeLists.txt                        # top-level
```

Notes:
- `core` contains only domain and ports (interfaces) plus use cases. No adapter includes here.
- `adapters` implement ports. Heavy/optional deps (OpenCascade, HTTP, remote logging) are build-guarded.
- `app/cli` wires everything together and provides IO/formatting. It depends on `usecase` and chosen adapters via DI. Future inbound adapters (desktop/web) reuse the same use case and ports.


## Interfaces and Contracts (sketch)

```
// core/ports/CadModelReaderPort.hpp
struct CadModelReaderPort {
  virtual ~CadModelReaderPort() = default;
  // Interpret CAD data from a binary/text stream into a domain Model
  virtual Model readModelFromStream(std::istream& stream) = 0;
};

// core/ports/ModelDataSourcePort.hpp
struct ModelDataSourcePort {
  virtual ~ModelDataSourcePort() = default;
  // Given a locator (e.g., file path, URL, or memory key), provide an input stream
  // Ownership of the stream is returned via unique_ptr
  virtual std::unique_ptr<std::istream> open(const std::string& locator) = 0;
};

// core/ports/LoggerPort.hpp
enum class LogLevel { Trace, Debug, Info, Warn, Error };
struct LoggerPort {
  virtual ~LoggerPort() = default;
  virtual void log(LogLevel level, const std::string& message) = 0;
};

// core/usecase/ListModelPartsUseCase.hpp
class ListModelPartsUseCase {
 public:
  ListModelPartsUseCase(ModelDataSourcePort& source, CadModelReaderPort& reader, LoggerPort& logger);
  std::vector<std::string> list(const std::string& locator) const; // or structured tree output
};
```

The domain `Model` would contain a root `Assembly` with child assemblies and parts. The use case can return a structured representation or a preformatted list of strings; the CLI `Formatter` handles final presentation (e.g., tree-like ASCII with stable ordering).


## Build (CMake) – target layering

- `cad_core` (INTERFACE): domain and ports headers
- `cad_usecases` (STATIC): use cases, depends on `cad_core`
- `adapter_fake` (STATIC): depends on `cad_core`
- `adapter_data_source_filesystem` (STATIC)
- `adapter_data_source_http` (STATIC, OPTIONAL)
- `adapter_logger_stdout`/`adapter_logger_file` (STATIC)
- `adapter_logger_remote` (STATIC, OPTIONAL)
- `adapter_opencascade` (STATIC, OPTIONAL)
- `cad_cli` (EXECUTABLE): depends on `cad_usecases` and selected adapters

Expose an option `CAD_ENABLE_OPENCASCADE` (default OFF or AUTO). If OFF, the CLI links to `adapter_fake` by default or accepts `--adapter=fake|opencascade`.


## Project Status Board

- [ ] Define domain types: `Model`, `Assembly`, `Part`, `Identifiers`
- [ ] Define outbound ports: `CadModelReaderPort`, `ModelDataSourcePort`, `LoggerPort`
- [ ] Implement `ListModelPartsUseCase`
- [ ] Add CLI inbound adapter: `main.cpp` and `Formatter`
- [ ] Add fake adapters and tests: model source, CAD reader, logger
- [ ] Add concrete adapters: filesystem source, stdout/file logger
- [ ] Scaffold CMake targets per layer
- [ ] Optional: Add OpenCascade adapter (guarded)
- [ ] Optional: Add HTTP data source (guarded)
- [ ] Optional: Add remote logger (guarded)
- [ ] Optional: Wire Nix flake devShell with cmake, test, and OpenCascade/HTTP


## Current Status / Progress Tracking

- Planning drafted; awaiting confirmation to proceed with scaffolding.


## Executor's Feedback or Assistance Requests

- Please confirm whether to include OpenCascade in the initial milestone or keep it optional behind a build flag and start with the fake adapter.
- Preferred test framework: Catch2 or GoogleTest?
- CLI interface: `cad-cli list <locator>` where locator can be a path or URL; add `--source=file|http|memory` (default file). Output format: tree with deterministic sorting.
- Logger defaults: `--logger=stdout` (default) with optional `--log-file=...`; later `--logger=remote`.


## Lessons

- Keep domain free of external types; map library-specific types inside adapters only.
- Favor deterministic ordering in output for stable tests.


