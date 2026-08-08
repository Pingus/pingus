{ self
, stdenv
, lib
, cmake
, tinycmmc_lib
}:

stdenv.mkDerivation {
  pname = "tinycmmc";
  version = tinycmmc_lib.versionFromVERSION self;

  src = lib.cleanSource ./.;

  nativeBuildInputs = [
    cmake
  ];
}
