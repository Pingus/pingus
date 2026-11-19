{ stdenv
, lib
, cmake
, glm
, gtest
, version
}:

stdenv.mkDerivation {
  pname = "geomcpp";
  inherit version;

  src = lib.cleanSource ./.;

  doCheck = true;

  cmakeFlags = [
    "-DWARNINGS=ON"
    "-DWERROR=ON"
    "-DBUILD_TESTS=ON"
    "-DPROJECT_VERSION_FULL=${version}"
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
