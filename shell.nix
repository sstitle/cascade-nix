{ pkgs ? import <nixpkgs> {} }:

let
  catch2Pkg = if pkgs ? catch2_3 then pkgs.catch2_3 else pkgs.catch2;
in pkgs.mkShell {
  # -----------------------------
  # Build tools
  # -----------------------------
  nativeBuildInputs = with pkgs; [
    cmake
    ninja
    pkg-config
    git
  ];

  # -----------------------------
  # C++ toolchain and libraries
  # -----------------------------
  buildInputs = with pkgs; [
    clang
    clang-tools
    catch2Pkg
    git
    nickel
    mask
    cmake
    ninja
    gcc
  ];

  # -----------------------------
  # Project tools (task runner, docs, etc.)
  # -----------------------------
  # Add non-essential tools here; keep the core C++ deps above cleanly separated.
  shellHook = ''
    echo "🚀 C++ dev shell loaded (shell.nix)"
    echo "Tools: cmake, ninja, clang/lld/libc++, catch2 (${catch2Pkg.pname or "catch2"} ${catch2Pkg.version or ""})"
    export CMAKE_GENERATOR=Ninja
    # Ensure CMake can find packages from Nix buildInputs
    export CMAKE_PREFIX_PATH="${catch2Pkg}:$CMAKE_PREFIX_PATH"
  '';
}


