{ stdenv
, lib
, libiconv
, cmake
}:

stdenv.mkDerivation {
  pname = "tinygettext";
  version = "0.2.0";

  src = lib.cleanSource ./.;

  cmakeFlags = [
    "-DTINYGETTEXT_UTF8_ONLY=ON"
  ];

  postFixup = ""
  + (lib.optionalString stdenv.hostPlatform.isWindows ''
    # This is rather ugly, but functional. Nix has a win-dll-link.sh
    # for this, but that's currently broken:
    # https://github.com/NixOS/nixpkgs/issues/38451
    mkdir -p $out/bin/

    ln -sv ${libiconv}/bin/*.dll $out/bin/
  '');

  nativeBuildInputs = [
    cmake
  ];

  propagatedBuildInputs = [
    libiconv
  ];
}
