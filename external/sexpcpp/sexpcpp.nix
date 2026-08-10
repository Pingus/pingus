{ stdenv
, lib
, cmake
, gtest
}:

stdenv.mkDerivation {
  pname = "sexp-cpp";
  version = "0.1.0";

  src = lib.cleanSource ./.;

  cmakeFlags = [
    "-DBUILD_TESTS=ON"
  ];

  doCheck = true;

  nativeBuildInputs = [
    cmake
  ];

  buildInputs = [
    gtest
  ];
}
