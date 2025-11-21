with import <nixpkgs> { };
mkShell {
  buildInputs = with xorg; [
    libX11
    libXinerama
    libXft
  ];
}
