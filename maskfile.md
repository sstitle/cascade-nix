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
