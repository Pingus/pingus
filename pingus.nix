{ self
, stdenv
, lib

, SDL2
, SDL2_image
, cmake
, fmt_8
, gtest
, libpng
, libsigcxx
, makeWrapper
, mesa
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
  version_file = lib.fileContents ./VERSION;
  pingus_version = if (((builtins.substring 0 1) version_file) != "v")
                   then ("0.8.0-${lib.substring 0 8 self.lastModifiedDate}-${self.shortRev or "dirty"}")
                   else (builtins.substring 1 ((builtins.stringLength version_file) - 2) version_file);
in
stdenv.mkDerivation {
  pname = "pingus";
  version = pingus_version;

  src = lib.cleanSource ./.;

  enableParallelBuilding = true;

  cmakeFlags = [
    "-DWARNINGS=ON"
    "-DWERROR=ON"
    "-DBUILD_EXTRA=OFF"
    "-DBUILD_TESTS=OFF" # tests fail due to SDLmain vs GTest::Main
  ];

  nativeBuildInputs = [
    cmake
    pkg-config
  ] ++ (lib.optional (!stdenv.targetPlatform.isWindows) makeWrapper);

  postFixup = ''
  ''
  + (lib.optionalString (!stdenv.targetPlatform.isWindows) ''
     wrapProgram $out/libexec/pingus \
       --prefix LIBGL_DRIVERS_PATH ":" "${mesa.drivers}/lib/dri" \
       --prefix LD_LIBRARY_PATH ":" "${mesa.drivers}/lib"
  '')
  + (lib.optionalString stdenv.targetPlatform.isWindows ''
     mkdir -p $out/bin/
     find ${mcfgthreads} -iname "*.dll" -exec ln -sfv {} $out/bin/ \;
     find ${stdenv.cc.cc} -iname "*.dll" -exec ln -sfv {} $out/bin/ \;
     ln -sfv ${SDL2}/bin/*.dll $out/bin/
     ln -sfv ${SDL2_image}/bin/*.dll $out/bin/
     ln -sfv ${fmt_8}/bin/*.dll $out/bin/
     ln -sfv ${gtest}/bin/*.dll $out/bin/
     ln -sfv ${libsigcxx}/bin/*.dll $out/bin/

     ln -sfv ${wstsound}/bin/*.dll $out/bin/
     ln -sfv ${tinygettext}/bin/*.dll $out/bin/
     ln -sfv ${priocpp}/bin/*.dll $out/bin/
   '');

  buildInputs = [
    SDL2
    SDL2_image
    fmt_8
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
  ++ lib.optional (!stdenv.targetPlatform.isWindows) xdgcpp;
}
