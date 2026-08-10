{ stdenv
, lib
, cmake
}:

stdenv.mkDerivation {
  pname = "uitest";
  version = "0.0.0";

  src = lib.cleanSource ./.;

  cmakeFlags = [
    "-DWARNINGS=ON"
    "-DWERROR=ON"
  ];

  nativeBuildInputs = [
    cmake
  ];
}
