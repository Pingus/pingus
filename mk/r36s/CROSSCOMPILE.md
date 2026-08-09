# Cross-compiling Pingus for R36S (ArkOS)

**Target hardware:** R36S handheld — Rockchip **RK3326** (4× Cortex-A35), **640×480** panel, Mali-G31 GPU.  
**Typical firmware:** [ArkOS](https://github.com/christianhaitian/arkos) (and community R36S images / dArkOS). ArkOS is a **modified Ubuntu 19.10 (eoan)** userspace for Rockchip handhelds, with **both aarch64 and armhf** library trees.

Pingus’s native resolution (**640×480**) matches the panel. Prefer windowed or fullscreen at that size; use `--renderer opengl` for GLES or `--renderer sdl` for the software path.

| Setting | Value |
|---------|--------|
| Video | **SDL2** + **OpenGL ES 2** (`-DPINGUS_USE_GLES=ON`) — Mali-G31 / libmali or Mesa |
| Sound | wstsound when available (`-DPINGUS_ENABLE_SOUND=ON`) |
| XDG | often off on CFW (`-DPINGUS_NO_XDGCPP=ON`) |
| ABI (preferred) | **aarch64-linux-gnu** |
| ABI (optional) | **arm-linux-gnueabihf** (PortMaster-style 32-bit) |

---

## Why a sysroot matters (glibc)

Stock ArkOS tracks **Ubuntu 19.10**-era libraries. That means roughly:

| Component | Typical on device |
|-----------|-------------------|
| glibc | **2.30** (eoan); some images stay near Debian Buster’s **2.28** |
| libstdc++ | GCC 8/9 era |
| SDL2 | Custom ArkOS builds (often 2.0.x with KMS/DRM patches), not stock Ubuntu only |

A binary linked against **modern nixpkgs / Ubuntu 22.04+ glibc** will fail on the device with:

```text
/lib/aarch64-linux-gnu/libc.so.6: version `GLIBC_2.3x' not found
```

So for a binary that runs on **stock ArkOS**, you must link against headers and shared libraries from a **matching or older** userspace (device copy, eoan/buster chroot, or equivalent sysroot).

Building with current `pkgsCross.aarch64-multiplatform` is still useful to prove the tree cross-compiles and to run under `qemu-aarch64`; it is **not** a drop-in ArkOS package without a compatible sysroot or shipping a full private `libs/` tree built for the same glibc.

---

## Recommended paths (pick one)

### A. Sysroot from the device (most accurate)

On a host with `aarch64-linux-gnu-gcc` (Debian/Ubuntu: `gcc-aarch64-linux-gnu`):

```bash
# On the R36S (SSH user often "ark" / password "ark"):
sudo apt-get update
sudo apt-get install --reinstall -y \
  libc6-dev linux-libc-dev libstdc++-9-dev \
  libsdl2-dev libsdl2-image-dev \
  libgles2-mesa-dev libegl1-mesa-dev libgbm-dev \
  zlib1g-dev libpng-dev pkg-config

# From the build machine:
./mk/r36s/scripts/make-sysroot-from-device.sh ark@<device-ip> $HOME/arkos-sysroot-aarch64
```

Then:

```bash
export ARKOS_SYSROOT=$HOME/arkos-sysroot-aarch64
cmake -S . -B build-r36s \
  -DCMAKE_TOOLCHAIN_FILE=mk/r36s/toolchain-arkos-aarch64.cmake \
  -DPINGUS_USE_GLES=ON -DPINGUS_ENABLE_SOUND=ON -DPINGUS_NO_XDGCPP=ON
cmake --build build-r36s -j"$(nproc)"
```

### B. Debootstrap / chroot sysroot

```bash
./mk/r36s/scripts/make-sysroot-debootstrap.sh $HOME/arkos-sysroot-aarch64
# then same cmake as above
```

### C. Nix flake (published ArkOS sysroot)

```bash
nix build .#arkos-sysroot          # unpack-only derivation
nix build .#pingus-r36s            # aarch64 binary + data
nix build .#pingus-r36s-portmaster # PortMaster tree under result/
nix build .#pingus-r36s-portmaster-zip
```

The flake pins an **ArkOS sysroot tarball** URL in `nix/r36s.nix`. Replace the
`localhost` placeholder with a real published URL (or override the `fetchurl`)
before a clean CI/hydra build. Refresh the hash with
`nix store prefetch-file <url>`.

---

## Toolchain files

| File | ABI |
|------|-----|
| `mk/r36s/toolchain-arkos-aarch64.cmake` | aarch64-linux-gnu |
| `mk/r36s/toolchain-arkos-armhf.cmake` | arm-linux-gnueabihf |

Both require `ARKOS_SYSROOT` (env or `-DARKOS_SYSROOT=`). They default
`PINGUS_USE_GLES`, `PINGUS_ENABLE_SOUND`, and `PINGUS_NO_XDGCPP` to ON; override
on the cmake command line if needed.

Cross compilers must be on `PATH` (`aarch64-linux-gnu-gcc` / `arm-linux-gnueabihf-gcc`).

---

## Deploy sketch (PortMaster / ES)

Preferred: build and install the PortMaster package:

```bash
nix build .#pingus-r36s-portmaster
cp -a result/"Pingus.sh" result/pingus /roms/ports/

# Or autoinstall zip:
nix build .#pingus-r36s-portmaster-zip
# copy result/pingus.zip → ports/PortMaster/autoinstall/
```

Manual layout:

```text
/roms/ports/
  Pingus.sh      # launcher (may source PortMaster control.txt)
  pingus/
    pingus       # binary
    data/        # game data
```

### Input

Pingus uses the **SDL Joystick API** (not GameController). Face buttons and
axes are the kernel’s raw indices; `SDL_GAMECONTROLLERCONFIG` does **not**
remap them inside Pingus.

PortMaster launchers still export `SDL_GAMECONTROLLERCONFIG` (GO-Super mapping)
for consistency with other ports and for any future GameController path.
Select+Start exit is typically provided by **gptokeyb** / **oga_controls** in
the launch script, not by Pingus itself.

Useful on-device checks:

```bash
# Prefer matching mali GBM EGL+GLES from one tree when mixing mesa/mali
export LD_LIBRARY_PATH=/usr/local/lib/aarch64-linux-gnu:$LD_LIBRARY_PATH
export SDL_VIDEODRIVER=KMSDRM
./pingus --renderer opengl --geometry 640x480
```

---

## References

- [ArkOS Building wiki](https://github.com/christianhaitian/ArkOS/wiki/Building) — Ubuntu 19.10 base, arm64/armhf chroots, Mali + SDL2 notes  
- [dov/r36s-programming](https://github.com/dov/r36s-programming) — on-device C/SDL2 setup, 640×480 hardware notes  
- [PortMaster packaging notes](https://github.com/christianhaitian/arkos/blob/main/ports/docs/packaging.md) — `libs/`, `SDL_GAMECONTROLLERCONFIG`, multi-device RK3326  
