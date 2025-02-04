{
  inputs = {
    utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, utils }: utils.lib.eachDefaultSystem (system:
    let
      pkgs = nixpkgs.legacyPackages.${system};
    in
    {
      devShell = pkgs.mkShell {
        buildInputs = with pkgs; [
          meson
          ninja
          gcc
          gcc_multi
          libtool
          gnumake
          bison
          flex
          texinfo
          xorriso
          qemu
          clang-tools
          OVMF
        ];

        shellHook = ''
          export PATH="$(readlink -f toolchain)/bin:$PATH"
          export OVMF_PATH=${pkgs.OVMF.fd}/FV/OVMF.fd
        '';    
      };
    }
  );
}
