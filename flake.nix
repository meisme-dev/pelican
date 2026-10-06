{
  inputs = {
    utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      nixpkgs,
      utils,
    }:
    utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      {
        devShell = pkgs.mkShell {
          nativeBuildInputs = [ pkgs.pkg-config ];
          buildInputs =
            with pkgs;
            [
              meson
              ninja
              gcc
              libtool
              gnumake
              bison
              flex
              texinfo
              xorriso
              qemu
              clang-tools
            ]
            ++ lib.optional stdenv.isx86_64 [
              OVMF
            ];

          shellHook = ''
            export PATH="$(readlink -f toolchain)/bin:$PATH"
            export CC=${pkgs.gcc}/bin/gcc
            export CXX=${pkgs.gcc}/bin/g++
          ''
          + pkgs.lib.optionalString pkgs.stdenv.isx86_64 ''
            export OVMF_PATH=${pkgs.OVMF.fd}/FV/OVMF.fd
          '';
        };
      }
    );
}
