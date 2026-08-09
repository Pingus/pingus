# R36S / ArkOS (RK3326) cross builds against a published ArkOS sysroot.
#
# The sysroot tarball supplies aarch64 headers + shared libs (glibc ~2.30 era,
# SDL2, GLES, …) so the binary can run on stock ArkOS — unlike pkgsCross
# alone, which links modern nixpkgs glibc/Mesa.
#
# Usage:
#   nix build .#arkos-sysroot
#   nix build .#pingus-r36s
#   nix build .#pingus-r36s-portmaster   # PortMaster tree for /roms/ports
#
{ lib
, stdenv
, stdenvNoCC
, fetchurl
, cmake
, pkg-config
, pkgsCross
, writeShellScript
, zip
, glm  # header-only; not present in ArkOS sysroot
}:

let
  arkosSysrootSrc = fetchurl {
    name = "arkos-sysroot.tar.gz";
    # Published sysroot (glibc ~2.30 + SDL2/GLES). If the hash drifts, run:
    #   nix store prefetch-file <url>
    # and paste the new sha256-… here.
    url = "https://github.com/grumnix/arkos-sysroot/releases/download/v0.1/arkos-sysroot.tar.gz";
    hash = "sha256-nIlMQ3P0uBrRQ9/k2x1s9DpdnF8iqA2wBLSB/20uXYg=";
  };

  # Allow hash to be overridden by the user who already fetched the tarball;
  # if the placeholder remains, Nix will print the expected hash.
  khrplatformH = ../mk/r36s/include/KHR/khrplatform.h;

  arkosSysroot = stdenvNoCC.mkDerivation {
    pname = "arkos-sysroot";
    version = "0.1";
    src = arkosSysrootSrc;

    # Unpack-only: aarch64 ELF + linker scripts must not be touched by the
    # host fixup (patchelf "wrong ELF type", strip, shebang rewrite).
    dontConfigure = true;
    dontBuild = true;
    dontFixup = true;
    dontPatchELF = true;
    dontStrip = true;
    dontPatchShebangs = true;
    dontCheckForBrokenSymlinks = true;

    installPhase = ''
      runHook preInstall
      mkdir -p "$out"

      if [ -d usr ]; then
        cp -a . "$out/"
      elif [ -d sysroot/usr ]; then
        cp -a sysroot/. "$out/"
      else
        top=
        for d in *; do
          if [ -d "$d/usr" ]; then top="$d"; break; fi
        done
        if [ -z "$top" ]; then
          echo "arkos-sysroot: unrecognized tarball layout (no usr/):" >&2
          find . -maxdepth 3 -type d >&2 || true
          exit 1
        fi
        cp -a "$top"/. "$out/"
      fi

      test -d "$out/usr" || {
        echo "arkos-sysroot: missing $out/usr after install" >&2
        exit 1
      }

      for base in "$out/usr/include" "$out/usr/lib" "$out/lib"; do
        if [ -d "$base/aarch64-linux-gnu" ] && [ ! -e "$base/aarch64-unknown-linux-gnu" ]; then
          ln -sfn aarch64-linux-gnu "$base/aarch64-unknown-linux-gnu"
        fi
      done

      mkdir -p "$out/usr/include/KHR"
      cp -f ${khrplatformH} "$out/usr/include/KHR/khrplatform.h"

      # Debian libc.so linker scripts embed absolute /lib/... paths. Rewrite
      # ONLY the multiarch absolute prefixes (not a bare "/lib/" which would
      # re-match inside /nix/store/.../lib/... and double the path).
      find "$out" -type f \( -name 'libc.so' -o -name 'libpthread.so' -o -name 'libm.so' -o -name 'libdl.so' -o -name 'librt.so' -o -name 'libutil.so' -o -name 'libresolv.so' -o -name 'libanl.so' -o -name 'libBrokenLocale.so' -o -name 'libthread_db.so' \) 2>/dev/null | while read -r f; do
        if grep -qE 'GROUP|INPUT' "$f" 2>/dev/null; then
          echo "patching linker script $f"
          # Match only when the path starts at a token boundary (space, (, =).
          sed -i -E \
            -e "s#(^|[[:space:](=])/usr/lib/aarch64-linux-gnu/#\1$out/usr/lib/aarch64-linux-gnu/#g" \
            -e "s#(^|[[:space:](=])/lib/aarch64-linux-gnu/#\1$out/lib/aarch64-linux-gnu/#g" \
            "$f" || true
        fi
      done

      ln -sfn . "$out/sysroot"
      echo "arkos-sysroot ready" > "$out/SYSROOT.txt"
      runHook postInstall
    '';

    meta = with lib; {
      description = "ArkOS / R36S aarch64 sysroot (glibc + SDL2 + GLES)";
      license = licenses.free;
      platforms = platforms.linux;
      hydraPlatforms = [];
    };
  };

  crossPkgs = pkgsCross.aarch64-multiplatform;
  crossCc = crossPkgs.stdenv.cc;
  targetPrefix = crossCc.targetPrefix;

  # Wrappers inject -nostdinc + ordered isystem so:
  #   1) libstdc++ *headers* from nixpkgs gcc (compile only)
  #   2) gcc fixed headers (stddef.h)
  #   3) ArkOS glibc headers only (never gcc's modern sys-include)
  # That avoids __attr_dealloc_free errors from mixing glibc 2.30 cdefs with
  # modern stdlib.h, and keeps #include_next <stdlib.h> working.
  #
  # Link is different: GCC 15's libstdc++/libgcc_s need GLIBC_2.32–2.38, but
  # ArkOS is ~2.30. g++ also injects an absolute path to its own libstdc++.so,
  # so -L order alone is ignored. We therefore:
  #   - compile with modern headers + _GLIBCXX_USE_CXX11_ABI=0 (old ABI)
  #   - -nostdlib++ so g++ does not force its libstdc++; link sysroot -lstdc++
  #   - -static-libgcc (libgcc.a has no versioned GLIBC_2.3x deps)
  #   - --allow-shlib-undefined for DT_NEEDED of sysroot libs (e.g. opusfile
  #     from SDL2_mixer) that exist on the device at runtime
  mkWrappers = sysroot: let
    gcc = crossCc.cc;
    tp = lib.removeSuffix "-" targetPrefix; # aarch64-unknown-linux-gnu
    libdir = "${sysroot}/usr/lib/aarch64-linux-gnu";
    cxxInc = "${gcc}/include/c++/${gcc.version}";
    cxxIncTarget = "${cxxInc}/${tp}";
    fixedInc = "${gcc}/lib/gcc/${tp}/${gcc.version}/include";
    fixedInc2 = "${gcc}/lib/gcc/${tp}/${gcc.version}/include-fixed";
    libgccDir = "${gcc}/lib/gcc/${tp}/${gcc.version}";
    gccLibOut = lib.getLib gcc;
    libgccLib = "${gccLibOut}/lib";
    libgccLibTarget = "${gccLibOut}/${tp}/lib";
    # Compile-only flags (safe with -c). No -L/-B lib paths that pull Scrt1.o.
    # -fno-exceptions: avoid GCC 15 libgcc_eh (_dl_find_object / GLIBC_2.35).
    # Milestone 1 does not rely on C++ exceptions.
    commonCompile = ''
      -nostdinc \
      --sysroot=${sysroot} \
      -isystem ${fixedInc} \
      -isystem ${fixedInc2} \
      -isystem ${sysroot}/usr/include/aarch64-linux-gnu \
      -isystem ${sysroot}/usr/include \
      -pthread \
      -fno-exceptions \
      -march=armv8-a \
      -mtune=cortex-a35 \
    '';
    commonCompileCxx = ''
      -nostdinc \
      -D_GLIBCXX_USE_CXX11_ABI=0 \
      --sysroot=${sysroot} \
      -isystem ${cxxInc} \
      -isystem ${cxxIncTarget} \
      -isystem ${cxxInc}/backward \
      -isystem ${fixedInc} \
      -isystem ${fixedInc2} \
      -isystem ${sysroot}/usr/include/aarch64-linux-gnu \
      -isystem ${sysroot}/usr/include \
      -pthread \
      -fno-exceptions \
      -march=armv8-a \
      -mtune=cortex-a35 \
    '';
    # Link flags: sysroot first for libc/SDL; add modern gcc -L only so
    # -shared-libgcc can find libgcc_s (stdc++ is still the absolute sysroot
    # path in the cxx wrapper — not -lstdc++).
    # Explicit dynamic linker so the binary runs on ArkOS (not /nix/store/.../ld).
    commonLink = ''
      --sysroot=${sysroot} \
      -Wl,--sysroot=${sysroot} \
      -Wl,--dynamic-linker=/lib/ld-linux-aarch64.so.1 \
      -B${libdir} \
      -B${libgccDir} \
      -L${libdir} \
      -L${sysroot}/usr/lib \
      -L${sysroot}/lib \
      -L${sysroot}/lib/aarch64-linux-gnu \
      -L${libgccDir} \
      -L${libgccLib} \
      -L${libgccLibTarget} \
      -shared-libgcc \
      -Wl,-Bdynamic \
      -pthread \
      -Wl,-rpath-link,${libdir} \
      -Wl,-rpath-link,${sysroot}/usr/lib/aarch64-linux-gnu \
      -Wl,-rpath-link,${sysroot}/lib/aarch64-linux-gnu \
      -Wl,-rpath-link,${sysroot}/usr/lib/aarch64-linux-gnu/pulseaudio \
      -Wl,-rpath-link,${sysroot}/lib/aarch64-linux-gnu/pulseaudio \
      -Wl,--allow-shlib-undefined \
      -Wl,--as-needed \
      -march=armv8-a \
      -mtune=cortex-a35 \
    '';
  in {
    cc = writeShellScript "aarch64-arkos-gcc" ''
      export PATH="${crossCc.bintools}/bin:$PATH"
      is_compile=
      for a in "$@"; do
        case "$a" in
          -c|-S|-E|-M|-MM|-MD|-MMD) is_compile=1 ;;
        esac
      done
      if [ -n "$is_compile" ]; then
        exec ${gcc}/bin/${targetPrefix}gcc \
          -B${crossCc.bintools}/bin \
          ${commonCompile} \
          "$@"
      else
        exec ${gcc}/bin/${targetPrefix}gcc \
          -B${crossCc.bintools}/bin \
          ${commonCompile} \
          ${commonLink} \
          "$@"
      fi
    '';
    # Link sysroot libstdc++ by absolute path so g++ cannot pick GCC 15's
    # (which requires GLIBCXX_3.4.32 not present on ArkOS).
    cxx = writeShellScript "aarch64-arkos-g++" ''
      export PATH="${crossCc.bintools}/bin:$PATH"
      is_compile=
      for a in "$@"; do
        case "$a" in
          -c|-S|-E|-M|-MM|-MD|-MMD) is_compile=1 ;;
        esac
      done
      if [ -n "$is_compile" ]; then
        exec ${gcc}/bin/${targetPrefix}g++ \
          -B${crossCc.bintools}/bin \
          ${commonCompileCxx} \
          "$@"
      else
        stdcpp=
        for cand in \
          "${libdir}/libstdc++.so" \
          "${libdir}/libstdc++.so.6" \
          "${sysroot}/usr/lib/libstdc++.so" \
          "${sysroot}/usr/lib/libstdc++.so.6" \
          "${sysroot}/lib/aarch64-linux-gnu/libstdc++.so" \
          "${sysroot}/lib/aarch64-linux-gnu/libstdc++.so.6"
        do
          if [ -e "$cand" ]; then stdcpp="$cand"; break; fi
        done
        if [ -z "$stdcpp" ]; then
          echo "aarch64-arkos-g++: no libstdc++ in sysroot" >&2
          exit 1
        fi
        exec ${gcc}/bin/${targetPrefix}g++ \
          -B${crossCc.bintools}/bin \
          ${commonCompileCxx} \
          -nostdlib++ \
          ${commonLink} \
          "$@" \
          -Wl,--no-as-needed "$stdcpp" -Wl,--as-needed
      fi
    '';
  };

  mkPingusR36s = {
    src
  , version
  , pname ? "pingus-r36s"
  # ArkOS sysroot has no OpenAL; wstsound's find_package(OpenAL) fails.
  # Keep OFF until OpenAL Soft is cross-built into the sysroot like Android.
  , enableSound ? false
  }:
    let
      wrappers = mkWrappers arkosSysroot;
    in
    stdenv.mkDerivation {
      inherit pname version src;

      nativeBuildInputs = [
        cmake
        pkg-config
        crossCc.bintools
      ];

      strictDeps = true;

      # Avoid host cmakeDefaults forcing the wrong compilers after our flags.
      cmakeFlags = [
        "-DCMAKE_SYSTEM_NAME=Linux"
        "-DCMAKE_SYSTEM_PROCESSOR=aarch64"
        "-DCMAKE_SYSROOT=${arkosSysroot}"
        "-DCMAKE_FIND_ROOT_PATH=${arkosSysroot}"
        "-DCMAKE_FIND_ROOT_PATH_MODE_PROGRAM=NEVER"
        "-DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY"
        "-DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=ONLY"
        "-DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=ONLY"
        "-DCMAKE_C_COMPILER=${wrappers.cc}"
        "-DCMAKE_CXX_COMPILER=${wrappers.cxx}"
        "-DCMAKE_C_COMPILER_WORKS=1"
        "-DCMAKE_CXX_COMPILER_WORKS=1"
        "-DCMAKE_C_COMPILER_FORCED=TRUE"
        "-DCMAKE_CXX_COMPILER_FORCED=TRUE"
        # Device binary must use ArkOS libs at runtime, not nix store paths.
        # Skipping RPATH rewrite also avoids cmake_install.cmake failing when
        # the linked RUNPATH does not contain the sysroot's /usr/lib/... path.
        "-DCMAKE_SKIP_RPATH=ON"
        "-DCMAKE_SKIP_INSTALL_RPATH=ON"
        "-DCMAKE_BUILD_WITH_INSTALL_RPATH=OFF"
        "-DCMAKE_INSTALL_RPATH="
        "-DCMAKE_BUILD_TYPE=Release"
        "-DBUILD_TESTS=OFF"
        "-DBUILD_EXTRA=OFF"
        "-DWARNINGS=OFF"
        "-DWERROR=OFF"
        # R36S is GLES2-only (Mali/Panfrost); desktop OpenGL is not in the sysroot.
        "-DPINGUS_USE_GLES=ON"
        # ArkOS sysroot has neither libsigc++ nor glm cmake config.
        "-DPINGUS_SIGC_POLYFILL_DIR=${../mk/android/app/jni}"
        "-DPINGUS_GLM_INCLUDE_DIR=${glm}/include"
        # Forced cross-compiler cannot try_compile pthread; ArkOS glibc has it.
        "-DCMAKE_HAVE_LIBC_PTHREAD=1"
        "-DCMAKE_THREAD_LIBS_INIT=-pthread"
        "-DPTHREAD_LIBRARY=pthread"
        # ArkOS sysroot lacks OpenAL; disable wstsound until Soft is packaged.
        "-DPINGUS_ENABLE_SOUND=${if enableSound then "ON" else "OFF"}"
        # Relative data next to the binary on device (PortMaster layout).
        "-DPROJECT_VERSION_FULL=${version}"
      ];

      # Do not let nix stdenv rewrite RUNPATH to modern glibc / gcc-15 libs,
      # or shebangs on helper scripts to /nix/store/.../bash.
      dontPatchELF = true;
      dontStrip = true;
      dontPatchShebangs = true;

      preConfigure = ''
        # Prevent stdenv from injecting -rpath to modern nixpkgs glibc/gcc.
        export NIX_DONT_SET_RPATH=1
        export NIX_NO_SELF_RPATH=1

        export PKG_CONFIG="pkg-config"
        export PKG_CONFIG_SYSROOT_DIR="${arkosSysroot}"
        export PKG_CONFIG_DIR=""
        export PKG_CONFIG_PATH=""
        export PKG_CONFIG_LIBDIR="${arkosSysroot}/usr/lib/aarch64-linux-gnu/pkgconfig:${arkosSysroot}/usr/lib/pkgconfig:${arkosSysroot}/usr/share/pkgconfig"
        pkg-config --exists sdl2 && pkg-config --modversion sdl2 || true

        ZLIB_LIB=
        for cand in \
          "${arkosSysroot}/usr/lib/aarch64-linux-gnu/libz.so" \
          "${arkosSysroot}/lib/aarch64-linux-gnu/libz.so" \
          "${arkosSysroot}/usr/lib/aarch64-linux-gnu/libz.so.1" \
          "${arkosSysroot}/lib/aarch64-linux-gnu/libz.so.1"
        do
          if [ -e "$cand" ]; then ZLIB_LIB="$cand"; break; fi
        done
        if [ -z "$ZLIB_LIB" ]; then
          echo "arkos-sysroot: no libz.so found" >&2
          exit 1
        fi
        cmakeFlagsArray+=(
          "-DZLIB_INCLUDE_DIR=${arkosSysroot}/usr/include"
          "-DZLIB_LIBRARY=$ZLIB_LIB"
        )

        # Explicit GLES/EGL from ArkOS sysroot (find_library can miss under ONLY mode).
        GLESV2_LIB=
        EGL_LIB=
        for cand in           "${arkosSysroot}/usr/lib/aarch64-linux-gnu/libGLESv2.so"           "${arkosSysroot}/lib/aarch64-linux-gnu/libGLESv2.so"           "${arkosSysroot}/usr/lib/libGLESv2.so"
        do
          if [ -e "$cand" ]; then GLESV2_LIB="$cand"; break; fi
        done
        for cand in           "${arkosSysroot}/usr/lib/aarch64-linux-gnu/libEGL.so"           "${arkosSysroot}/lib/aarch64-linux-gnu/libEGL.so"           "${arkosSysroot}/usr/lib/libEGL.so"
        do
          if [ -e "$cand" ]; then EGL_LIB="$cand"; break; fi
        done
        if [ -z "$GLESV2_LIB" ]; then
          echo "arkos-sysroot: no libGLESv2.so found" >&2
          find "${arkosSysroot}" -name 'libGLESv2*' 2>/dev/null | head -20 >&2 || true
          exit 1
        fi
        cmakeFlagsArray+=(
          "-DPINGUS_GLESV2_LIB=$GLESV2_LIB"
        )
        if [ -n "$EGL_LIB" ]; then
          cmakeFlagsArray+=(
            "-DPINGUS_EGL_LIB=$EGL_LIB"
          )
        fi
      '';

      postInstall = ''
        mkdir -p $out/share/pingus
        # CMake may have installed data/ as non-writable; ensure we can add files.
        chmod -R u+w $out/share/pingus || true
        if [ -d "$src/data" ]; then
          cp -a "$src/data/." $out/share/pingus/ || true
          chmod -R u+w $out/share/pingus || true
        fi
        cat > $out/share/pingus/README-R36S.txt << EOF_README
Pingus — R36S / ArkOS (sysroot-linked)
====================================================

Binary: libexec/pingus (or bin/pingus)
  Linked against the ArkOS aarch64 sysroot (SDL2 + OpenGL/GLES as available).

Deploy the binary + share/pingus data to the device.

Controls (important)
--------------------
SDL2 only opens SDL GameController devices. The R36S GO-Super Gamepad is
often joystick-only unless SDL_GAMECONTROLLERCONFIG is set.

  Preferred: nix build .#pingus-r36s-portmaster
  and install under /roms/ports/ (launcher sources PortMaster control.txt).

  Manual: export the GO-Super mapping, then run with -v and confirm
  "[pad] … controller" (not "joystick-only"). See mk/r36s/CROSSCOMPILE.md.
EOF_README
        cat > $out/share/pingus/pingus.sh << 'LAUNCH'
#!/bin/bash
DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR"
BIN="$DIR/../bin/pingus"
if [ ! -x "$BIN" ]; then BIN="$DIR/pingus"; fi
# Without PortMaster control.txt, set SDL_GAMECONTROLLERCONFIG for GO-Super
# or the pad stays joystick-only (see mk/r36s/CROSSCOMPILE.md).
exec "$BIN" --fullscreen "$@"
LAUNCH
        chmod +x $out/share/pingus/pingus.sh
      '';

      meta = with lib; {
        description = "Pingus for R36S/ArkOS (sysroot-linked aarch64)";
        license = licenses.gpl3Plus;
        platforms = platforms.linux;
        hydraPlatforms = [];
      };
    };

  # Wrap a mkPingusR36s result as a PortMaster-ready tree:
  #   Pingus.sh + pingus/ + metadata
  # Copy into /roms/ports/ or zip for PortMaster autoinstall.
  mkPingusR36sPortMaster = {
    r36sPkg
  , version
  , pname ? "pingus-r36s-portmaster"
  , title ? "Pingus"
  , scriptName ? "Pingus.sh"
  , portDirName ? "pingus"
  , screenshotSrc ? ../data/icons/hicolor/128x128/apps/pingus.png
  }:
    stdenvNoCC.mkDerivation {
      inherit pname version;
      dontUnpack = true;
      dontConfigure = true;
      dontBuild = true;
      # Device must run the launcher with ArkOS /bin/bash — never rewrite
      # shebangs to a nix store path (causes "bad interpreter" on R36S).
      dontPatchShebangs = true;
      dontFixup = true;

      # Binary is already cross-built; this is only packaging.
      nativeBuildInputs = [ ];

      installPhase = ''
        set -euo pipefail
        root="$out"
        gamedir="$root/${portDirName}"
        mkdir -p "$gamedir/data" "$gamedir/licenses" "$gamedir/conf"

        # Binary from the R36S derivation
        install -m755 "${r36sPkg}/bin/pingus" \
          "$gamedir/pingus"

        # Game data (CMake DATA_PREFIX was share/pingus).
        # Store paths are mode 444/555; make writable so we can drop helpers.
        if [ -d "${r36sPkg}/share/pingus" ]; then
          cp -a "${r36sPkg}/share/pingus/." "$gamedir/data/"
          chmod -R u+w "$gamedir/data"
          # Keep only game assets under data/; drop packaging helpers
          rm -f "$gamedir/data/pingus.sh" \
                "$gamedir/data/README-R36S.txt"
        fi

        # Placeholder / project icon as screenshot + cover (PortMaster wants
        # 4:3 gameplay ideally; icon is fine until a real capture is added).
        if [ -f "${screenshotSrc}" ]; then
          cp -f "${screenshotSrc}" "$root/screenshot.png"
          cp -f "${screenshotSrc}" "$root/cover.png"
          cp -f "${screenshotSrc}" "$gamedir/cover.png"
        else
          # Minimal valid 1x1 PNG if the asset is missing
          printf '\x89PNG\r\n\x1a\n\x00\x00\x00\rIHDR\x00\x00\x00\x01\x00\x00\x00\x01\x08\x02\x00\x00\x00\x90wS\xde\x00\x00\x00\x0cIDATx\x9cc\xf8\x0f\x00\x00\x01\x01\x00\x05\x18\xd8N\x00\x00\x00\x00IEND\xaeB`\x82' \
            > "$root/screenshot.png"
          cp -f "$root/screenshot.png" "$root/cover.png"
          cp -f "$root/screenshot.png" "$gamedir/cover.png"
        fi

        # License stubs
        if [ -d "${r36sPkg}/share/licenses" ]; then
          cp -a "${r36sPkg}/share/licenses/." "$gamedir/licenses/" || true
        fi
        cat > "$gamedir/licenses/README.txt" << 'EOF_LIC'
Pingus — see upstream GPL-3.0-or-later and LICENSES/ in the source tree.
This PortMaster package redistributes the game binary and data for ArkOS/R36S.
EOF_LIC

        # PortMaster launch script (sources control.txt → SDL_GAMECONTROLLERCONFIG)
        cat > "$root/${scriptName}" << 'EOF_LAUNCH'
#!/bin/bash
# Pingus — PortMaster launcher for R36S / ArkOS

XDG_DATA_HOME=''${XDG_DATA_HOME:-$HOME/.local/share}

if [ -d "/opt/system/Tools/PortMaster/" ]; then
  controlfolder="/opt/system/Tools/PortMaster"
elif [ -d "/opt/tools/PortMaster/" ]; then
  controlfolder="/opt/tools/PortMaster"
elif [ -d "$XDG_DATA_HOME/PortMaster/" ]; then
  controlfolder="$XDG_DATA_HOME/PortMaster"
else
  controlfolder="/roms/ports/PortMaster"
fi

source "$controlfolder/control.txt"
[ -f "''${controlfolder}/mod_''${CFW_NAME}.txt" ] && source "''${controlfolder}/mod_''${CFW_NAME}.txt"
get_controls

GAMEDIR="/$directory/ports/pingus"
CONFDIR="$GAMEDIR/conf"

mkdir -p "$CONFDIR"
# SD/FAT copies can leave files non-writable for user "ark"
chmod -R u+rwX "$GAMEDIR" 2>/dev/null || true
cd "$GAMEDIR" || exit 1

if [ -w "$GAMEDIR" ]; then
  > "$GAMEDIR/log.txt" 2>/dev/null && exec > >(tee -a "$GAMEDIR/log.txt") 2>&1 || true
fi

export XDG_DATA_HOME="$CONFDIR"
export XDG_CONFIG_HOME="$CONFDIR"
export SDL_GAMECONTROLLERCONFIG="$sdl_controllerconfig"

# Native aarch64 SDL2 gamecontroller input — no gptokeyb needed.
pm_platform_helper "$GAMEDIR/pingus" 2>/dev/null || true

# Force on-device data + config dirs (do not use any baked-in install prefix).
./pingus \
  --datadir "$GAMEDIR/data" \
  --userdir "$CONFDIR" \
  --fullscreen \
  "$@"
pm_finish 2>/dev/null || true
EOF_LAUNCH
        chmod +x "$root/${scriptName}"

        # port.json (PortMaster catalog / autoinstall metadata)
        cat > "$root/port.json" << EOF_JSON
{
  "version": 2,
  "name": "pingus.zip",
  "items": [
    "${scriptName}",
    "${portDirName}"
  ],
  "items_opt": null,
  "attr": {
    "title": "${title}",
    "desc": "Classic Pingus (SDL2 + GLES2) for ArkOS / R36S. Free jump-and-run platformer starring Tux.",
    "inst": "Ready to run. Copy Pingus.sh and the pingus/ folder into /roms/ports/ (or install the zip via PortMaster autoinstall).",
    "genres": ["platform", "action"],
    "porter": ["Pingus-Origins"],
    "image": {},
    "rtr": true,
    "runtime": null,
    "reqs": [],
    "arch": ["aarch64"]
  }
}
EOF_JSON

        # EmulationStation metadata (cover used as list art)
        cat > "$root/gameinfo.xml" << EOF_XML
<?xml version="1.0" encoding="utf-8"?>
<gameList>
  <game>
    <path>./${scriptName}</path>
    <name>${title}</name>
    <desc>Classic Pingus — free jump-and-run platformer starring Tux. SDL2 + GLES2 build for ArkOS / R36S.</desc>
    <releasedate>20040511T000000</releasedate>
    <developer>Pingus Team</developer>
    <publisher>Pingus-Origins</publisher>
    <genre>Platform</genre>
    <image>./${portDirName}/cover.png</image>
  </game>
</gameList>
EOF_XML

        cat > "$root/README.md" << 'EOF_README'
## Pingus (R36S / ArkOS)

Native **aarch64** build linked against the ArkOS sysroot (SDL2 + GLES2).

### Install

1. Copy `Pingus.sh` and the `pingus/` directory to `/roms/ports/` on the device, **or**
2. Zip this folder and place the zip in `ports/PortMaster/autoinstall/`, then open PortMaster once.

### Controls (required)

This build uses **SDL2 GameController** only. The R36S built-in **GO-Super
Gamepad** does not expose a mapping by default when the binary is started
outside PortMaster; you will see:

    Warning: Joystick(s) present but none have a gamecontroller mapping.

**This PortMaster launcher** sources `control.txt` (`get_controls`) so
`SDL_GAMECONTROLLERCONFIG` is set for the device. Launch via EmulationStation
**Ports** (or the `.sh` script), not by running the binary alone over SSH
without that env.

If you must run the binary directly, export a GO-Super mapping first (see
`mk/r36s/CROSSCOMPILE.md`) and use `-v` until the log shows
`[pad] … controller "GO-Super Gamepad"`.

In-game defaults: D-pad / left stick move, **A** jump, **B** run/fire, **Start** menu.

### Credits

Thanks to the Pingus developers and the PortMaster / ArkOS communities.
EOF_README

        '';

      meta = with lib; {
        description = "PortMaster package of Pingus for R36S/ArkOS";
        license = licenses.gpl3Plus;
        platforms = platforms.linux;
        hydraPlatforms = [];
      };
    };

  # Zip of the PortMaster tree for autoinstall (ports/PortMaster/autoinstall/).
  # Flat archive: launcher + port dir + metadata at zip root (matches port.json).
  mkPingusR36sPortMasterZip = {
    portMasterPkg
  , version
  , pname ? "pingus-r36s-portmaster-zip"
  , zipName ? "pingus.zip"
  }:
    stdenvNoCC.mkDerivation {
      inherit pname version;
      dontUnpack = true;
      dontConfigure = true;
      dontBuild = true;
      dontFixup = true;
      dontPatchShebangs = true;

      nativeBuildInputs = [ zip ];

      installPhase = ''
        set -euo pipefail
        mkdir -p "$out"
        ( cd "${portMasterPkg}" && zip -r -9 "$out/${zipName}" . )
      '';

      meta = with lib; {
        description = "PortMaster autoinstall zip of Pingus for R36S/ArkOS";
        license = licenses.gpl3Plus;
        platforms = platforms.linux;
        hydraPlatforms = [];
      };
    };

in
{
  inherit arkosSysroot mkPingusR36s mkPingusR36sPortMaster mkPingusR36sPortMasterZip;
}
