{
  description = "Development environment with nickel and mask";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-25.05";
    flake-utils.url = "github:numtide/flake-utils";
    treefmt-nix.url = "github:numtide/treefmt-nix";
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
      treefmt-nix,
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        treefmtEval = treefmt-nix.lib.evalModule pkgs ./nix/treefmt.nix;
      in
      {
        # Development shell with C++ toolchain, cmake, and mask
        devShells.default = pkgs.mkShell {
          buildInputs = with pkgs; [
            # Core tools
            git
            nickel
            mask
            cmake
            ninja
            gcc
          ];

          shellHook = ''
            echo "🚀 Development environment loaded!"
            echo "Available tools:"
            echo "  - cmake: Build system"
            echo "  - ninja: Build tool"
            echo "  - gcc/clang: C++ toolchain"
            echo "  - nickel: Configuration language"
            echo "  - mask: Task runner"
            echo ""
            echo "Run 'mask --help' to see available tasks."
            echo "Run 'nix fmt' to format all files."
            export CMAKE_GENERATOR=Ninja
          '';
        };

        # for `nix fmt`
        formatter = treefmtEval.config.build.wrapper;

        # for `nix flake check`
        checks = {
          formatting = treefmtEval.config.build.check self;
        };
      }
    );
}
