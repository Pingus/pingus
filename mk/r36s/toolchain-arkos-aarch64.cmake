# CMake toolchain: aarch64 Pingus → R36S / ArkOS
#
# Usage:
#   export ARKOS_SYSROOT=/path/to/aarch64-rootfs
#   cmake -S . -B build-r36s \
#     -DCMAKE_TOOLCHAIN_FILE=mk/r36s/toolchain-arkos-aarch64.cmake \
#     -DPINGUS_USE_GLES=ON -DPINGUS_ENABLE_SOUND=ON
#
# See mk/r36s/CROSSCOMPILE.md

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

if(NOT ARKOS_SYSROOT AND DEFINED ENV{ARKOS_SYSROOT})
  set(ARKOS_SYSROOT "$ENV{ARKOS_SYSROOT}")
endif()
if(NOT ARKOS_SYSROOT)
  message(FATAL_ERROR
    "ARKOS_SYSROOT is not set. Point it at an aarch64 rootfs with SDL2/GLES dev files.\n"
    "See mk/r36s/CROSSCOMPILE.md")
endif()

get_filename_component(ARKOS_SYSROOT "${ARKOS_SYSROOT}" ABSOLUTE)
set(CMAKE_SYSROOT "${ARKOS_SYSROOT}")
set(CMAKE_FIND_ROOT_PATH "${ARKOS_SYSROOT}")

set(CMAKE_C_COMPILER   aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)
set(CMAKE_AR           aarch64-linux-gnu-ar CACHE FILEPATH "" FORCE)
set(CMAKE_RANLIB       aarch64-linux-gnu-ranlib CACHE FILEPATH "" FORCE)
set(CMAKE_STRIP        aarch64-linux-gnu-strip CACHE FILEPATH "" FORCE)
set(CMAKE_PKG_CONFIG_EXECUTABLE aarch64-linux-gnu-pkg-config CACHE FILEPATH "")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(ENV{PKG_CONFIG_SYSROOT_DIR} "${ARKOS_SYSROOT}")
set(ENV{PKG_CONFIG_LIBDIR}
  "${ARKOS_SYSROOT}/usr/lib/aarch64-linux-gnu/pkgconfig:${ARKOS_SYSROOT}/usr/lib/pkgconfig:${ARKOS_SYSROOT}/usr/share/pkgconfig")
set(ENV{PKG_CONFIG_PATH} "")

# RK3326 = 4× Cortex-A35. Override with -DCMAKE_C_FLAGS_INIT=… if needed.
set(CMAKE_C_FLAGS_INIT   "-march=armv8-a -mtune=cortex-a35")
set(CMAKE_CXX_FLAGS_INIT "-march=armv8-a -mtune=cortex-a35")

# Defaults for this handheld (callers may still override on the command line).
set(PINGUS_USE_GLES      ON  CACHE BOOL "OpenGL ES 2.0 (Mali-G31 / KMSDRM)" FORCE)
set(PINGUS_ENABLE_SOUND  ON  CACHE BOOL "wstsound audio" FORCE)
set(PINGUS_NO_XDGCPP     ON  CACHE BOOL "No xdgcpp on embedded CFW" FORCE)
