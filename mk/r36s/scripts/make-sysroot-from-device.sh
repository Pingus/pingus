#!/usr/bin/env bash
# Assemble a minimal ArkOS/R36S sysroot by rsync from a live device.
#
# Usage:
#   ./make-sysroot-from-device.sh ark@192.168.1.50 ~/arkos-sysroot-aarch64
#   ./make-sysroot-from-device.sh ark@192.168.1.50 ~/arkos-sysroot-armhf armhf
#
# Requires: ssh, rsync. Device should have -dev packages installed
# (see mk/r36s/CROSSCOMPILE.md).

set -euo pipefail

REMOTE="${1:?usage: $0 user@host DEST [aarch64|armhf]}"
DEST="${2:?destination directory}"
ARCH="${3:-aarch64}"

case "$ARCH" in
  aarch64|arm64)
    LIBTRIPLE=aarch64-linux-gnu
    ;;
  armhf|arm)
    LIBTRIPLE=arm-linux-gnueabihf
    ;;
  *)
    echo "Unknown arch: $ARCH (use aarch64 or armhf)" >&2
    exit 1
    ;;
esac

mkdir -p "$DEST"
echo "==> Syncing headers and $LIBTRIPLE libraries from $REMOTE → $DEST"

# Keep paths under DEST identical to on-device layout so CMAKE_SYSROOT works.
rsync -a --info=progress2 \
  --exclude='**/gconv/**' \
  --exclude='**/audit/**' \
  "${REMOTE}:/usr/include/" "${DEST}/usr/include/"

rsync -a --info=progress2 \
  "${REMOTE}:/usr/lib/${LIBTRIPLE}/" "${DEST}/usr/lib/${LIBTRIPLE}/" \
  || rsync -a --info=progress2 \
       "${REMOTE}:/usr/lib/" "${DEST}/usr/lib/"

# Multi-arch linker scripts often reference /lib; pull common shared objects.
if ssh "$REMOTE" "test -d /lib/${LIBTRIPLE}"; then
  rsync -a --info=progress2 \
    "${REMOTE}:/lib/${LIBTRIPLE}/" "${DEST}/lib/${LIBTRIPLE}/"
fi

# pkg-config data
if ssh "$REMOTE" "test -d /usr/lib/${LIBTRIPLE}/pkgconfig"; then
  mkdir -p "${DEST}/usr/lib/${LIBTRIPLE}/pkgconfig"
  rsync -a "${REMOTE}:/usr/lib/${LIBTRIPLE}/pkgconfig/" \
    "${DEST}/usr/lib/${LIBTRIPLE}/pkgconfig/"
fi

# Record provenance
{
  echo "source=${REMOTE}"
  echo "arch=${ARCH}"
  echo "libtriple=${LIBTRIPLE}"
  echo "date=$(date -u +%Y-%m-%dT%H:%M:%SZ)"
  ssh "$REMOTE" "uname -a; ldd --version 2>&1 | head -1; \
    dpkg -l 'libsdl2-2.0-0' 'libc6' 2>/dev/null | awk '/^ii/{print}'"
} > "${DEST}/SYSROOT_FROM_DEVICE.txt"

echo "==> Done. Export ARKOS_SYSROOT=${DEST}"
echo "    cmake -S . -B build-r36s -DCMAKE_TOOLCHAIN_FILE=mk/r36s/toolchain-arkos-${ARCH/arm64/aarch64}.cmake ..."
