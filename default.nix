with import <nixpkgs> { };
stdenv.mkDerivation rec {
  name = "local-dwm-${version}";
  version = "6.4.0";

  src = ./.;

  buildInputs = with xorg; [
    libX11
    libXinerama
    libXft
    xorg.xorgserver
    # clang
  ];

  unpackPhase = ''cp -r $src/* .'';

  buildPhase = ''make'';

  installPhase = ''make PREFIX=$out DESTDIR="" install'';
}
