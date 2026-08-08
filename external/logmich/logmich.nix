{ stdenv
, lib
, cmake

, tinycmmc
}:

stdenv.mkDerivation {
  pname = "logmich";
  version = "0.2.0";

  src = lib.cleanSource ./.;

  nativeBuildInputs = [
    cmake
  ];

  buildInputs = [
    tinycmmc
  ];

}
