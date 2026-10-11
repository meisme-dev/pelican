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
        pkgsLinux_x86_64 = nixpkgs.legacyPackages.x86_64-linux;
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
            ];

          shellHook = ''
            export PATH="$(readlink -f toolchain)/bin:$PATH"
            export CC=${pkgs.gcc}/bin/gcc
            export CXX=${pkgs.gcc}/bin/g++
            export OVMF_PATH=${pkgsLinux_x86_64.OVMF.fd}/FV/OVMF.fd
          '';
        };
      }
    );
}
