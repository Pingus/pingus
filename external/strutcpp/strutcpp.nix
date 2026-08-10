{ stdenv
, lib
, cmake
, gtest
}:

stdenv.mkDerivation {
  pname = "strutcpp";
  version = "0.0.0";

  src = lib.cleanSource ./.;

  cmakeFlags = [
    "-DBUILD_TESTS=ON"
  ];

  doCheck = true;

  checkPhase = ''
    make test ARGS="--verbose"
  '';

  nativeBuildInputs = [
    cmake
  ];

  buildInputs = [
    gtest
  ];
}
