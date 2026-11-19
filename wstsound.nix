{ stdenv
, lib
, cmake
, gtest
, mcfgthreads ? null
, libmodplug ? null
, libogg ? null
, libvorbis ? null
, mpg123 ? null
, openal
, opusfile ? null
, libopus ? null
, withModplug ? true
, withVorbis  ? true
, withOpus    ? true
, withMpg123  ? true
, withEfx     ? true
, buildExtra  ? true
, buildTests  ? true
, version ? "0.3.0"
}:

assert withModplug -> libmodplug != null;
assert withVorbis  -> libogg != null && libvorbis != null;
assert withOpus    -> libogg != null && opusfile != null && libopus != null;
assert withMpg123  -> mpg123 != null;

stdenv.mkDerivation {
  pname = "wstsound";
  inherit version;

  src = ./.;

  cmakeFlags = [
    "-DWARNINGS=ON"
    "-DWERROR=ON"
    "-DBUILD_TESTS=${if buildTests then "ON" else "OFF"}"
    "-DBUILD_EXTRA=${if buildExtra then "ON" else "OFF"}"
    "-DWSTSOUND_WITH_MODPLUG=${if withModplug then "ON" else "OFF"}"
    "-DWSTSOUND_WITH_VORBIS=${if withVorbis then "ON" else "OFF"}"
    "-DWSTSOUND_WITH_OPUS=${if withOpus then "ON" else "OFF"}"
    "-DWSTSOUND_WITH_MPG123=${if withMpg123 then "ON" else "OFF"}"
    "-DWSTSOUND_WITH_EFX=${if withEfx then "ON" else "OFF"}"
    "-DPROJECT_VERSION_FULL=${version}"
  ];

  postFixup = ""
  + (lib.optionalString stdenv.hostPlatform.isWindows (''
    # This is rather ugly, but functional. Nix has a win-dll-link.sh
    # for this, but that's currently broken:
    # https://github.com/NixOS/nixpkgs/issues/38451
    mkdir -p $out/bin/

    find ${mcfgthreads} -iname "*.dll" -exec ln -sfv {} $out/bin/ \;
    find ${stdenv.cc.cc} -iname "*.dll" -exec ln -sfv {} $out/bin/ \;
    ln -sfv ${openal}/bin/*.dll $out/bin/
  ''
  + lib.optionalString withModplug ''
    ln -sfv ${libmodplug}/bin/*.dll $out/bin/
  ''
  + lib.optionalString withVorbis ''
    ln -sfv ${libogg}/bin/*.dll $out/bin/
    ln -sfv ${libvorbis}/bin/*.dll $out/bin/
  ''
  + lib.optionalString withOpus ''
    ln -sfv ${libogg}/bin/*.dll $out/bin/
    ln -sfv ${opusfile}/bin/*.dll $out/bin/
    ln -sfv ${libopus}/bin/*.dll $out/bin/
  ''
  + lib.optionalString withMpg123 ''
    ln -sfv ${mpg123}/bin/*.dll $out/bin/
  ''
  ));

  nativeBuildInputs = [
    cmake
  ];

  buildInputs = lib.optionals buildTests [
    gtest
  ];

  propagatedBuildInputs = [
    openal
  ]
  ++ lib.optionals withModplug [ libmodplug ]
  ++ lib.optionals withVorbis  [ libogg libvorbis ]
  ++ lib.optionals withOpus    [ libogg opusfile libopus ]
  ++ lib.optionals withMpg123  [ mpg123 ];
}
