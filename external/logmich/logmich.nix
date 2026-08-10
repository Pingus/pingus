{ stdenv
, lib
, cmake
}:

stdenv.mkDerivation {
  pname = "logmich";
  version = "0.2.0";

  src = lib.cleanSource ./.;

  cmakeFlags = [
    "-DWARNINGS=ON"
    "-DWERROR=ON"
  ];

  nativeBuildInputs = [
    cmake
  ];
}
