{ stdenv
, lib
, cmake
, tinycmmc
, gtest
}:

stdenv.mkDerivation {
  pname = "sexp-cpp";
  version = "0.1.0";

  src = lib.cleanSource ./.;

  cmakeFlags = [
    "-DBUILD_TESTS=ON"
  ]
  ++ (lib.optional (!stdenv.hostPlatform.isWindows) "-DUSE_CXX17=ON");

  doCheck = true;

  nativeBuildInputs = [
    cmake
  ];

  buildInputs = [
    tinycmmc
    gtest
  ];
}
