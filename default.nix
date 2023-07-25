with (import <nixpkgs> {});
mkShell {
  buildInputs = [
    cmake
    ninja
    gcc
    xorriso
    qemu
  ];
}