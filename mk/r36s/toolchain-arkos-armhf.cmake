# CMake toolchain: armhf Pingus → R36S / ArkOS (optional PortMaster-style ABI)
#
# Usage:
#   export ARKOS_SYSROOT=/path/to/armhf-rootfs
#   cmake -S . -B build-r36s-hf \
#     -DCMAKE_TOOLCHAIN_FILE=mk/r36s/toolchain-arkos-armhf.cmake \
#     -DPINGUS_USE_GLES=ON -DPINGUS_ENABLE_SOUND=ON
#
# See mk/r36s/CROSSCOMPILE.md

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

if(NOT ARKOS_SYSROOT AND DEFINED ENV{ARKOS_SYSROOT})
  set(ARKOS_SYSROOT "$ENV{ARKOS_SYSROOT}")
endif()
if(NOT ARKOS_SYSROOT)
  message(FATAL_ERROR
    "ARKOS_SYSROOT is not set. Point it at an armhf rootfs with SDL2/GLES dev files.\n"
    "See mk/r36s/CROSSCOMPILE.md")
endif()

get_filename_component(ARKOS_SYSROOT "${ARKOS_SYSROOT}" ABSOLUTE)
set(CMAKE_SYSROOT "${ARKOS_SYSROOT}")
set(CMAKE_FIND_ROOT_PATH "${ARKOS_SYSROOT}")

set(CMAKE_C_COMPILER   arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)
set(CMAKE_AR           arm-linux-gnueabihf-ar CACHE FILEPATH "" FORCE)
set(CMAKE_RANLIB       arm-linux-gnueabihf-ranlib CACHE FILEPATH "" FORCE)
set(CMAKE_STRIP        arm-linux-gnueabihf-strip CACHE FILEPATH "" FORCE)
set(CMAKE_PKG_CONFIG_EXECUTABLE arm-linux-gnueabihf-pkg-config CACHE FILEPATH "")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(ENV{PKG_CONFIG_SYSROOT_DIR} "${ARKOS_SYSROOT}")
set(ENV{PKG_CONFIG_LIBDIR}
  "${ARKOS_SYSROOT}/usr/lib/arm-linux-gnueabihf/pkgconfig:${ARKOS_SYSROOT}/usr/lib/pkgconfig:${ARKOS_SYSROOT}/usr/share/pkgconfig")
set(ENV{PKG_CONFIG_PATH} "")

set(CMAKE_C_FLAGS_INIT   "-march=armv7-a -mfpu=neon-vfpv4 -mfloat-abi=hard")
set(CMAKE_CXX_FLAGS_INIT "-march=armv7-a -mfpu=neon-vfpv4 -mfloat-abi=hard")

set(PINGUS_USE_GLES      ON  CACHE BOOL "OpenGL ES 2.0 (Mali-G31 / KMSDRM)" FORCE)
set(PINGUS_ENABLE_SOUND  ON  CACHE BOOL "wstsound audio" FORCE)
set(PINGUS_NO_XDGCPP     ON  CACHE BOOL "No xdgcpp on embedded CFW" FORCE)
