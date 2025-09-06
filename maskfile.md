# Maskfile

This is a [mask](https://github.com/jacobdeichert/mask) task runner file.

## hello

> This is an example command you can run with `mask hello`

```bash
echo "Hello World!"
```

## build

> Configure and build the C++ demo

```bash
cmake --preset default
cmake --build --preset default
```

## run:mem

> Run the CLI with an in-memory locator

```bash
./build/cpp/cad/cad_cli list mem:demo
```

## test

> Build and run tests (CTest + Catch2)

```bash
cmake --preset default
cmake --build --preset default
CDPATH= ctest --test-dir build --output-on-failure
```

## demo

> Demonstrate all adapter combinations and functionality

```bash
set -e

echo "🚀 Building project..."
cmake --preset default
cmake --build --preset default

echo ""
echo "🧪 Running all tests..."
CDPATH= ctest --test-dir build --output-on-failure

echo ""
echo "📋 Demonstrating all CLI combinations:"
echo ""

echo "1️⃣  Default (fake data source + fake logger):"
./build/cpp/cad/cad_cli list mem:demo
echo ""

echo "2️⃣  Fake data source + spdlog logger:"
./build/cpp/cad/cad_cli --logger=spdlog list mem:demo
echo ""

echo "3️⃣  JSON data source + fake logger (simple model):"
./build/cpp/cad/cad_cli --data-source=json list test-data/simple_model.json
echo ""

echo "4️⃣  JSON data source + spdlog logger (basic model):"
./build/cpp/cad/cad_cli --logger=spdlog --data-source=json list test-data/test_model.json
echo ""

echo "5️⃣  Complex nested JSON model with spdlog:"
./build/cpp/cad/cad_cli --logger=spdlog --data-source=json list test-data/complex_model.json
echo ""

echo "6️⃣  Error handling (missing file):"
./build/cpp/cad/cad_cli --data-source=json list test-data/missing.json || echo "✅ Error handling works correctly"
echo ""

echo "🎉 All examples completed successfully!"
echo ""
echo "📖 Available CLI options:"
echo "  --logger=fake|spdlog      Choose logging implementation"
echo "  --data-source=fake|json   Choose data source implementation"
echo ""
echo "📁 New adapter structure:"
echo "  cpp/cad/adapters/"
echo "  ├── logger/               (fake outputs to console, spdlog with timestamps)"
echo "  ├── model-data-source/    (fake in-memory, json file-based)"
echo "  └── cad-model-reader/     (fake text format, json flat structure)"
```
