with (import <nixpkgs> {});
mkShell {
  buildInputs = [
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
    clang-tools
  ];
  
  shellHook = ''
    export PATH="$(readlink -f toolchain)/bin:$PATH"
  '';
}