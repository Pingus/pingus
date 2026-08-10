{ stdenv
, lib
, cmake
, gtest
, mcfgthreads
, libmodplug
, libogg
, libvorbis
, mpg123
, openal
, opusfile
, libopus
, tinycmmc
}:

stdenv.mkDerivation {
  pname = "wstsound";
  version = "0.3.0";

  src = ./.;

  cmakeFlags = [
    "-DWARNINGS=ON"
    "-DWERROR=ON"
    "-DBUILD_TESTS=ON"
    "-DBUILD_EXTRA=ON"
  ];

  postFixup = ""
  + (lib.optionalString stdenv.hostPlatform.isWindows ''
    # This is rather ugly, but functional. Nix has a win-dll-link.sh
    # for this, but that's currently broken:
    # https://github.com/NixOS/nixpkgs/issues/38451
    mkdir -p $out/bin/

    find ${mcfgthreads} -iname "*.dll" -exec ln -sfv {} $out/bin/ \;
    find ${stdenv.cc.cc} -iname "*.dll" -exec ln -sfv {} $out/bin/ \;
    ln -sfv ${libmodplug}/bin/*.dll $out/bin/
    ln -sfv ${libogg}/bin/*.dll $out/bin/
    ln -sfv ${libvorbis}/bin/*.dll $out/bin/
    ln -sfv ${mpg123}/bin/*.dll $out/bin/
    ln -sfv ${openal}/bin/*.dll $out/bin/
    ln -sfv ${opusfile}/bin/*.dll $out/bin/
    ln -sfv ${libopus}/bin/*.dll $out/bin/
  '');

  nativeBuildInputs = [
    cmake
  ];

  buildInputs = [
    tinycmmc
    gtest
  ];

  propagatedBuildInputs = [
    libmodplug
    libogg
    libvorbis
    mpg123
    openal
    opusfile
    libopus
  ];
}
