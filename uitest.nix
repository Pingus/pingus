{ stdenv
, lib
, cmake
, tinycmmc
}:

stdenv.mkDerivation {
  pname = "uitest";
  version = "0.0.0";

  src = lib.cleanSource ./.;

  nativeBuildInputs = [
    cmake
  ];

  buildInputs = [
    tinycmmc
  ];
}
