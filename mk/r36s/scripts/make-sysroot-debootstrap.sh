#!/usr/bin/env bash
# Build a Debian Buster sysroot (glibc 2.28) for ArkOS-compatible cross links.
#
# Usage (root or fakeroot + qemu-user-static recommended):
#   sudo ./make-sysroot-debootstrap.sh /opt/arkos-sysroot-buster arm64
#   sudo ./make-sysroot-debootstrap.sh /opt/arkos-sysroot-buster-hf armhf
#
# Host packages: debootstrap, qemu-user-static, binfmt-support
# (Debian/Ubuntu).

set -euo pipefail

DEST="${1:?usage: $0 DEST arm64|armhf}"
ARCH="${2:-arm64}"

case "$ARCH" in
  arm64|aarch64) DEBARCH=arm64; LIBTRIPLE=aarch64-linux-gnu ;;
  armhf|arm)     DEBARCH=armhf; LIBTRIPLE=arm-linux-gnueabihf ;;
  *) echo "arch must be arm64 or armhf" >&2; exit 1 ;;
esac

if [[ "$(id -u)" -ne 0 ]]; then
  echo "Run as root (debootstrap needs privileges)." >&2
  exit 1
fi

MIRROR="${DEBIAN_MIRROR:-http://deb.debian.org/debian}"
SUITE=buster

mkdir -p "$DEST"
if [[ ! -e "$DEST/usr/bin/apt-get" ]]; then
  echo "==> debootstrap $SUITE $DEBARCH → $DEST"
  debootstrap --arch="$DEBARCH" --variant=minbase \
    --include=build-essential,pkg-config,libsdl2-dev,libsdl2-image-dev,libsdl2-mixer-dev,libgles2-mesa-dev,libegl1-mesa-dev,libgbm-dev,libdrm-dev,zlib1g-dev,libpng-dev \
    "$SUITE" "$DEST" "$MIRROR"
else
  echo "==> Existing rootfs at $DEST — installing/updating build deps"
  chroot "$DEST" apt-get update
  chroot "$DEST" apt-get install -y --no-install-recommends \
    build-essential pkg-config \
    libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev \
    libgles2-mesa-dev libegl1-mesa-dev libgbm-dev libdrm-dev \
    zlib1g-dev libpng-dev
fi

{
  echo "suite=${SUITE}"
  echo "debarch=${DEBARCH}"
  echo "libtriple=${LIBTRIPLE}"
  echo "mirror=${MIRROR}"
  echo "date=$(date -u +%Y-%m-%dT%H:%M:%SZ)"
  echo "note=glibc 2.28 floor; safe for Ubuntu 19.10 / ArkOS linking"
} > "${DEST}/SYSROOT_DEBOOTSTRAP.txt"

echo "==> Sysroot ready: ARKOS_SYSROOT=${DEST}"
echo "    Host still needs: gcc-${LIBTRIPLE%-gnu*} (or ${LIBTRIPLE}-gcc)"
