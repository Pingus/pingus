# TODO — Pingus ports, flake, and external/ work

Living checklist for the current development push (git subtrees, Nix flake
cleanup, experimental WASM / Android / R36S). Update this file when items
land or scope changes.

## Done

### Process / repo hygiene

- [x] Add `AGENTS.md` (coding standards, git-bundle handoffs, AI attribution)
- [x] Clarify handoffs: **only** `git bundle` + `git pull`; never `git fetch` /
      `git cherry-pick` as a substitute for stacking
- [x] Flake Windows outputs under **host** system (`pingus-win32-x64` /
      `pingus-win32-x86`), not `packages.*-windows`
- [x] Drop `x86_64-darwin` from `eachSystem` (nixpkgs unstable dropped it)

### `external/` subtrees

- [x] Vendor helper libraries under `external/` via `git subtree add --squash`
  - tinycmmc, uitest, argpp, geomcpp, logmich, sexpcpp, priocpp, strutcpp,
    tinygettext, xdgcpp, wstsound
- [x] Avoid recursive submodule checkout when adding subtrees
- [x] Nested upstream gitlinks (`external/*/external/tinycmmc`) left as
      gitlinks only — real modules live at `external/tinycmmc`

### CMake + Nix using `external/`

- [x] `tinycmmc_find_dependency`: monorepo paths + aliases
      (`geom`→`geomcpp`, `prio`→`priocpp`, `strut`→`strutcpp`, `sexp`→`sexpcpp`)
- [x] Bootstrap `TinyCMMC.cmake` falls back to top-level `external/tinycmmc`
- [x] Hardcoded module paths in argpp/logmich/strut/wstsound fixed for monorepo
- [x] `sexp` ordered before `prio` for in-tree builds
- [x] Flake builds helpers with `callPackage` from `external/*/` (no flake
      inputs for those libs)
- [x] Flake inputs reduced to nixpkgs, flake-utils, Win32 SDL ports (+ SDL
      source tarballs for Android/wasm)

### Experimental ports (scaffolding from SuperTux Milestone 1)

- [x] Copy/adapt `mk/{wasm,android,r36s}` and `nix/{wasm,android,r36s}.nix`
      (GP2X/Wiz **not** brought over)
- [x] Wire host-system packages: `pingus-wasm`, `pingus-android`,
      `pingus-r36s`, PortMaster zips, `arkos-sysroot`, SDL helper outputs
- [x] CMake: `SDL2_ROOT`, `PINGUS_USE_GLES`, `EMSCRIPTEN_LINK_FLAGS`,
      html suffix, skip `xdgcpp` on Emscripten/Android
- [x] `PINGUS_ENABLE_SOUND` (forced OFF on Emscripten): dummy audio, no wstsound
- [x] wasm `build-app.sh` uses Pingus CMake options (not SuperTux `ENABLE_*`)

## In progress / next

### Dependencies

- [x] Replace `{fmt}` with `std::format` / `std::vformat` (C++20) in Pingus,
      logmich, and priocpp
- [ ] Provide **glm** + **libsigc++-2** for Emscripten (`EXTRA_PREFIX_PATH`)
      and confirm `find_package` / pkg-config under FIND_ROOT BOTH


### WASM (`.#pingus-wasm`)

- [x] Strip SuperTux-only `EXPORTED_FUNCTIONS` (`_st_emscripten_*`) from the
      link line; keep `_main` + main-loop pause/resume only
- [x] ~~Legacy GL emulation~~ superseded by real GLES2 shaders (`FULL_ES2` only)
- [x] Soften `mk/wasm/shell.html` when optional `_st_emscripten_*` symbols
      are absent (guards already present; exports no longer require them)
- [ ] Get a full `emcmake` + link of `pingus` past compile/link errors
- [ ] Runtime: main loop / filesystem (`data/` preload at `/data`), canvas
      resize, input
- [ ] Optional: restore sound via SDL2_mixer wasm + a thin backend (long-term)

### Android (`.#pingus-android`)

- [x] Stage `external/*/include` into the NDK tree; exclude `sound_real.cpp`
- [ ] Build/link `external/` static libs for each ABI — not only headers
- [x] GLES context attributes in OpenGLFramebuffer when `PINGUS_USE_GLES`
- [ ] Replace placeholder launcher icons with Pingus art
- [ ] Validate APK install on a device/emulator

### R36S / ArkOS (`.#pingus-r36s`)

- [ ] Successful CMake configure against `arkos-sysroot` (SDL2, GL/GLES,
      fmt, glm, sigc++, libpng, …)
- [ ] Build `external/` for aarch64-sysroot or rely on sysroot packages
- [ ] Binary runs under PortMaster with `SDL_GAMECONTROLLERCONFIG`
- [ ] Real screenshot/cover assets for PortMaster metadata

### Shared engine work (unblocks WASM + Android + ES devices)

- [x] Request ES 2.0 context via SDL when `PINGUS_USE_GLES` / Emscripten
- [x] Compile-time `PINGUS_DEFAULT_DATADIR` from CMake `DATA_PREFIX` (wasm preload at `/data`)
- [x] **Shader-based OpenGL path** shared by **OpenGL 3.3 Core** (desktop default)
      and **OpenGL ES 2.0** (`PINGUS_USE_GLES` / Emscripten): `opengl_program.*`,
      VBO draws, no fixed-function / no legacy GL emulation
- [ ] Runtime verification on desktop GL 3.3, wasm GLES2, Android GLES2
- [ ] Runtime verification of data path (`DATA_PREFIX` / preload) on each port

### Flake / packaging polish

- [ ] Run `nix flake lock` after input changes; prune unused lock nodes
- [ ] CI: `nix build .#pingus` + optional port smoke jobs
- [ ] Consider vendoring Win32 SDL under `external/` later (optional)

## Out of scope (for now)

- GP2X / Wiz / Open2x (explicitly excluded)
- Archive write, remote VFS, or other non-port features
- Pixel-perfect SuperTux Milestone 1 feature parity (different game)

## Quick commands

```sh
nix build .#pingus
nix build .#pingus-win32-x64
nix build .#wasm-sdl-libs          # SDL stack only
nix build .#pingus-wasm            # full wasm app (WIP)
nix build .#arkos-sysroot
nix build .#pingus-r36s            # WIP
nix build .#pingus-android         # WIP
```

## Reference

- SuperTux Milestone 1 ports (source of packaging patterns):
  https://github.com/SuperTux-Origins/supertux-milestone1
- Pingus repo: https://github.com/Pingus/pingus
