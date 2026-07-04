{ self
, stdenv
, lib
, tinycmmc_lib

, SDL2
, SDL2_image
, cmake
, fmt
, gtest
, libpng
, libsigcxx
, makeWrapper
, libGL
, libGLU
, pkg-config
, mcfgthreads

, argpp
, geomcpp
, logmich
, priocpp
, strutcpp
, tinycmmc
, tinygettext
, uitest
, wstsound
, xdgcpp
}:

let
  pingus_version = tinycmmc_lib.versionFromVERSION self;
in
stdenv.mkDerivation rec {
  pname = "pingus";
  version = pingus_version;

  src = lib.cleanSource ./.;

  enableParallelBuilding = true;

  cmakeFlags = [
    "-DWARNINGS=ON"
    "-DWERROR=ON"
    "-DBUILD_EXTRA=OFF"
    "-DBUILD_TESTS=OFF" # tests fail due to SDLmain vs GTest::Main
    "-DPROJECT_VERSION_FULL=${pingus_version}"
  ];

  nativeBuildInputs = [
    cmake
    pkg-config
  ] ++ (lib.optional (!stdenv.hostPlatform.isWindows) makeWrapper);

  preConfigure = ''
    echo "$version" > VERSION
  '';

  postFixup = ''
  ''
  + (lib.optionalString stdenv.hostPlatform.isWindows ''
     mkdir -p $out/bin/
     find ${mcfgthreads} -iname "*.dll" -exec ln -sfv {} $out/bin/ \;
     find ${stdenv.cc.cc} -iname "*.dll" -exec ln -sfv {} $out/bin/ \;
     ln -sfv ${SDL2}/bin/*.dll $out/bin/
     ln -sfv ${SDL2_image}/bin/*.dll $out/bin/
     ln -sfv ${fmt}/bin/*.dll $out/bin/
     ln -sfv ${gtest}/bin/*.dll $out/bin/
     ln -sfv ${libsigcxx}/bin/*.dll $out/bin/

     ln -sfv ${wstsound}/bin/*.dll $out/bin/
     ln -sfv ${tinygettext}/bin/*.dll $out/bin/
     ln -sfv ${priocpp}/bin/*.dll $out/bin/
   '');

  buildInputs = [
    libGL
    libGLU

    SDL2
    SDL2_image
    fmt
    gtest
    libpng
    libsigcxx

    argpp
    geomcpp
    logmich
    priocpp
    strutcpp
    tinycmmc
    tinygettext
    uitest
    wstsound
  ]
  ++ lib.optional (!stdenv.hostPlatform.isWindows) xdgcpp;
}
