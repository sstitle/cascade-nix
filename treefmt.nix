{ pkgs, lib, ... }:
{
  projectRootFile = "flake.nix";
  programs = {
    # keep-sorted start
    clang-format.enable = true;
    cmake-format.enable = true;
    keep-sorted.enable = true;
    nixfmt.enable = true;
    # keep-sorted end

    mdformat = {
      enable = true;
      package = pkgs.mdformat.withPlugins (
        ps: with ps; [
          # keep-sorted start
          mdformat-footnote
          mdformat-frontmatter
          mdformat-gfm
          # keep-sorted end
          (mdformat-toc.overrideAttrs {
            # Needed to patch unit test that is failing otherwise - keep-sorted start
            doCheck = false;
            doInstallCheck = false;
            meta.broken = false;
            pytestCheckPhase = "";
            # keep-sorted end
          })
        ]
      );
    };

  };
  settings.formatter = {
    cmake-format.includes = lib.mkAfter [
      "*.cmake"
      "*/CMakeLists.txt"
    ];
  };
}
