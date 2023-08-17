with (import <nixpkgs> {});
mkShell {
  buildInputs = [
    cmake
    meson
    ninja
    gcc
    gcc_multi
    gnumake
    bison
    flex
    texinfo
    cloog
    xorriso
    qemu
  ];
  
  shellHook = ''
    export PATH="$(readlink -f toolchain)/bin:$PATH"
  '';
}