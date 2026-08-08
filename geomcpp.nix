{ self
, stdenv
, lib
, cmake
, glm
, gtest
, tinycmmc
, tinycmmc_lib
}:

stdenv.mkDerivation {
  pname = "geomcpp";
  version = tinycmmc_lib.versionFromFile self;

  src = lib.cleanSource ./.;

  postPatch = ''
    echo "v${tinycmmc_lib.versionFromFile self}" > VERSION
  '';

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
    tinycmmc
  ];

  propagatedBuildInputs = [
    glm
  ];
}
