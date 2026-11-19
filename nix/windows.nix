# SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
# SPDX-License-Identifier: GPL-3.0-or-later

# SDL3 and SDL3_image for the Windows (mingw-w64) cross builds, compiled
# here from nixpkgs' sources: nixpkgs' own mingw SDL packages pull in X11,
# which does not cross-build, and SDL needs nothing external on Windows.
# SDL3_image uses its built-in stb_image (loading) and miniz (IMG_SavePNG)
# backends, so no libpng/zlib DLLs need to be shipped.
#
#   nix build .#sdl3-win64 .#sdl3-image-win64

{ pkgs        # native, for tools and the sources
, crossPkgs   # pkgsCross.mingwW64 / mingw32
, target      # "win64" / "win32"
}:

let
  stdenv = crossPkgs.stdenv;

  commonFlags = [
    "-DCMAKE_BUILD_TYPE=Release"
  ];

  sdl3 = stdenv.mkDerivation {
    pname = "sdl3-${target}";
    inherit (pkgs.sdl3) version src;
    nativeBuildInputs = [ pkgs.cmake pkgs.ninja ];
    cmakeFlags = commonFlags ++ [
      "-DSDL_SHARED=ON"
      "-DSDL_STATIC=OFF"
      "-DSDL_TESTS=OFF"
      "-DSDL_TEST_LIBRARY=OFF"
      "-DSDL_EXAMPLES=OFF"
      "-DSDL_REVISION=SDL-${pkgs.sdl3.version}"
    ];
  };

  sdl3-image = stdenv.mkDerivation {
    pname = "sdl3-image-${target}";
    inherit (pkgs.sdl3-image) version src;
    nativeBuildInputs = [ pkgs.cmake pkgs.ninja ];
    buildInputs = [ sdl3 ];
    cmakeFlags = commonFlags ++ [
      "-DSDL3_DIR=${sdl3}/lib/cmake/SDL3"
      "-DBUILD_SHARED_LIBS=ON"
      "-DSDLIMAGE_SAMPLES=OFF"
      "-DSDLIMAGE_TESTS=OFF"
      "-DSDLIMAGE_VENDORED=OFF"
      "-DSDLIMAGE_BACKEND_STB=ON"
      "-DSDLIMAGE_BACKEND_WIC=OFF"
      # stb_image loads PNGs, miniz writes them (IMG_SavePNG).
      "-DSDLIMAGE_PNG_LIBPNG=OFF"
      # Formats needing external libraries; Pingus' data is PNG and JPEG.
      "-DSDLIMAGE_AVIF=OFF"
      "-DSDLIMAGE_JXL=OFF"
      "-DSDLIMAGE_TIF=OFF"
      "-DSDLIMAGE_WEBP=OFF"
    ];
  };

in {
  inherit sdl3 sdl3-image;
}
