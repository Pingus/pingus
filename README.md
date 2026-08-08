Pingus
======

Introduction
------------

Pingus is a free Lemmings™ clone for GNU/Linux, Win32 and other
operating systems supported by SDL2. It is licensed under the GPLv3+.

Information about the game, new versions and additional documentation
can be found at the Pingus webpage:

* https://pingus.github.io/


Building
--------

Pingus uses [Nix package manager](https://nixos.org/download/), to
automatically download, compile and run it in Linux:

    nix run github:Pingus/pingus

Windows cross builds are produced from a Linux host (packages live under
the *host* system, not under `packages.*-windows`):

    nix build github:Pingus/pingus#pingus-win32-x64   # mingwW64 → x86_64 PE
    nix build github:Pingus/pingus#pingus-win32-x86   # mingw32  → i686 PE

Zipped redistributables:

    nix build github:Pingus/pingus#pingus-win32-x64-zip
    nix build github:Pingus/pingus#pingus-win32-x86-zip


Mailing Lists
-------------

* pingus-devel - https://lists.nongnu.org/mailman/listinfo/pingus-devel
  A list for discussing development issues, like level
  creation or code changes, but might as well be used for user issues
  like how to solve levels and such


Chat
----

* IRC: irc://irc.libera.chat:6667/#pingus

* Matrix: https://matrix.to/#/#pingus:matrix.org


Bug Reports
-----------

Bug reports, new levels and feature requests can be submitted to the
issue tracker at:

* https://github.com/pingus/pingus/issues

Ports (experimental)
--------------------

Scaffolding adapted from SuperTux Milestone 1. These packages live under the
*host* system (e.g. `packages.x86_64-linux.…`):

    nix build .#pingus-r36s              # R36S / ArkOS aarch64 (sysroot)
    nix build .#pingus-r36s-portmaster   # PortMaster tree for /roms/ports
    nix build .#pingus-android           # APK (SDL layer + NDK)
    nix build .#pingus-wasm              # emscripten + preloaded data/

**Status:** Nix packaging plus CMake hooks for Emscripten (`SDL2_ROOT`,
`PINGUS_USE_GLES`, `PINGUS_ENABLE_SOUND`, html suffix / link flags) and R36S
sysroot builds. WASM defaults to dummy audio (no wstsound). Full GLES2 /
WebGL rendering and NDK builds of `external/` are still incomplete — expect
compile errors in the OpenGL fixed-function path until that is ported.
Prefer desktop `.#pingus` and Windows cross for day-to-day builds.

See `mk/r36s/CROSSCOMPILE.md`, `mk/android/README.md`, `mk/wasm/README.md`.

