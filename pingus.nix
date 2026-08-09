{ self
, stdenv
, lib
, tinycmmc_lib

, SDL2
, SDL2_image
, cmake

, gtest

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
, useGLES2 ? false
, libglvnd ? null
, addDriverRunpath ? null
}:

let
  pingus_version = tinycmmc_lib.versionFromVERSION self;
in
stdenv.mkDerivation rec {
  pname = if useGLES2 then "pingus-gles2" else "pingus";
  version = pingus_version;

  src = lib.cleanSourceWith {
    src = ./.;
    filter = path: type:
      let base = baseNameOf path; in
      !(base == ".git" || base == "result" || base == "build"
        || lib.hasSuffix ".bundle" base);
  };

  enableParallelBuilding = true;

  cmakeFlags = [
    "-DWARNINGS=ON"
    "-DWERROR=ON"
    "-DBUILD_EXTRA=OFF"
    "-DBUILD_TESTS=OFF" # tests fail due to SDLmain vs GTest::Main
    "-DPROJECT_VERSION_FULL=${pingus_version}"
    "-DPINGUS_USE_GLES=${if useGLES2 then "ON" else "OFF"}"
  ] ++ lib.optionals stdenv.hostPlatform.isWindows [
    # No pkg-config sigc++-2.0 on MinGW in this flake; use Android header polyfill.
    "-DPINGUS_SIGC_POLYFILL_DIR=${./mk/android/app/jni}"
  ];

  nativeBuildInputs = [
    cmake
    pkg-config
  ] ++ (lib.optional (!stdenv.hostPlatform.isWindows) makeWrapper)
    ++ (lib.optional (useGLES2 && addDriverRunpath != null) addDriverRunpath);

  preConfigure = ''
    echo "$version" > VERSION
  '';

  postFixup = ''
  ''
  + (lib.optionalString (useGLES2 && !stdenv.hostPlatform.isWindows && addDriverRunpath != null) ''
     addDriverRunpath $out/bin/pingus
  '')
  + (lib.optionalString stdenv.hostPlatform.isWindows ''
     mkdir -p $out/bin/
     find ${mcfgthreads} -iname "*.dll" -exec ln -sfv {} $out/bin/ \;
     find ${stdenv.cc.cc} -iname "*.dll" -exec ln -sfv {} $out/bin/ \;
     ln -sfv ${SDL2}/bin/*.dll $out/bin/
     ln -sfv ${SDL2_image}/bin/*.dll $out/bin/
     ln -sfv ${wstsound}/bin/*.dll $out/bin/ || true
     ln -sfv ${tinygettext}/bin/*.dll $out/bin/ || true
     ln -sfv ${priocpp}/bin/*.dll $out/bin/ || true
   '');

  buildInputs = [
    SDL2
    SDL2_image

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
  ++ lib.optionals (!stdenv.hostPlatform.isWindows) [
    libGL
    libGLU
    gtest
    libsigcxx
    xdgcpp
  ]
  ++ lib.optional (useGLES2 && libglvnd != null) libglvnd;
}
