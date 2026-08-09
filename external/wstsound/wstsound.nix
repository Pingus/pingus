{ stdenv
, lib
, cmake
, gtest
, mcfgthreads ? null
, libmodplug
, openal
, tinycmmc
  # Optional codecs (native Linux). Windows ships openal + modplug only.
, libogg ? null
, libvorbis ? null
, mpg123 ? null
, opusfile ? null
, libopus ? null
}:

let
  win = stdenv.hostPlatform.isWindows;
  # Desktop data is mostly WAV + module music; full codec set is Linux-only.
  slimCodecs = win;
in
stdenv.mkDerivation {
  pname = "wstsound";
  version = "0.3.0";

  src = ./.;

  cmakeFlags = [
    "-DWARNINGS=ON"
    "-DWERROR=ON"
    "-DBUILD_TESTS=${if win then "OFF" else "ON"}"
    "-DBUILD_EXTRA=${if win then "OFF" else "ON"}"
  ] ++ lib.optionals slimCodecs [
    "-DWSTSOUND_WITH_MPG123=OFF"
    "-DWSTSOUND_WITH_VORBIS=OFF"
    "-DWSTSOUND_WITH_OPUS=OFF"
    "-DWSTSOUND_WITH_MODPLUG=ON"
    "-DWSTSOUND_WITH_EFX=OFF"
  ];

  postFixup = ""
  + (lib.optionalString win ''
    mkdir -p $out/bin/
    find ${mcfgthreads} -iname "*.dll" -exec ln -sfv {} $out/bin/ \;
    find ${stdenv.cc.cc} -iname "*.dll" -exec ln -sfv {} $out/bin/ \;
    ln -sfv ${libmodplug}/bin/*.dll $out/bin/ || true
    ln -sfv ${openal}/bin/*.dll $out/bin/ || true
  '');

  nativeBuildInputs = [
    cmake
  ];

  buildInputs = [
    tinycmmc
  ] ++ lib.optional (!win) gtest;

  propagatedBuildInputs = [
    libmodplug
    openal
  ] ++ lib.optionals (!slimCodecs) [
    libogg
    libvorbis
    mpg123
    opusfile
    libopus
  ];
}
