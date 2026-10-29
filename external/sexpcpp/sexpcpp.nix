{ stdenv
, lib
, cmake
, gtest
, version
}:

stdenv.mkDerivation {
  pname = "sexp-cpp";
  inherit version;

  src = lib.cleanSource ./.;

  cmakeFlags = [
    "-DBUILD_TESTS=ON"
    "-DWARNINGS=ON"
    "-DWERROR=ON"
    "-DPROJECT_VERSION_FULL=${version}"
  ];

  doCheck = true;

  nativeBuildInputs = [
    cmake
  ];

  buildInputs = [
    gtest
  ];
}
