{ self
, stdenv
, lib
, cmake
, glm
, gtest
}:

stdenv.mkDerivation {
  pname = "geomcpp";
  version = lib.strings.removeSuffix "\n" (builtins.readFile ./VERSION);

  src = lib.cleanSource ./.;

  doCheck = true;

  cmakeFlags = [
    "-DWARNINGS=ON"
    "-DWERROR=ON"
    "-DBUILD_TESTS=ON"
  ];

  nativeBuildInputs = [
    cmake
  ];

  buildInputs = [
    gtest
  ];

  propagatedBuildInputs = [
    glm
  ];
}
