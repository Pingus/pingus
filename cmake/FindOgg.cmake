# Copyright (C) 2021 Ingo Ruhnke <grumbel@gmail.com>
#
# This software is provided 'as-is', without any express or implied
# warranty.  In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented; you must not
#    claim that you wrote the original software. If you use this software
#    in a product, an acknowledgment in the product documentation would be
#    appreciated but is not required.
# 2. Altered source versions must be plainly marked as such, and must not be
#    misrepresented as being the original software.
# 3. This notice may not be removed or altered from any source distribution.

find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
  pkg_search_module(PC_OGG ogg)
endif()

find_path(OGG_INCLUDE_DIRECTORY "ogg/ogg.h"
  PATHS ${OGG_DIR} ${PC_OGG_INCLUDE_DIRS}
  PATH_SUFFIXES "include"
  )

find_library(OGG_LIBRARY ogg
  PATHS ${OGG_DIR} ${PC_OGG_LIBRARY_DIRS}
  PATH_SUFFIXES "lib"
  )

# Drop pkg-config include dirs that point at vanished build trees
# (e.g. /build/libogg-*/prefix/include from a previous configure).
if(PC_OGG_INCLUDE_DIRS)
  set(_ogg_pc_incs "")
  foreach(_d ${PC_OGG_INCLUDE_DIRS})
    if(EXISTS "${_d}")
      list(APPEND _ogg_pc_incs "${_d}")
    endif()
  endforeach()
  set(PC_OGG_INCLUDE_DIRS "${_ogg_pc_incs}")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Ogg
  REQUIRED_VARS OGG_INCLUDE_DIRECTORY OGG_LIBRARY
  )

# Idempotent: Opus + Vorbis both find_package(Ogg); re-running must not
# try to create Ogg::ogg a second time (CMake error: target already exists).
if(NOT TARGET Ogg::ogg)
  add_library(Ogg::ogg UNKNOWN IMPORTED)
  set_target_properties(Ogg::ogg
    PROPERTIES
    IMPORTED_LOCATION "${OGG_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${OGG_INCLUDE_DIRECTORY};${PC_OGG_INCLUDE_DIRS}"
    INTERFACE_LINK_LIBRARIES "${PC_OGG_LINK_LIBRARIES}"
    INTERFACE_LINK_OPTIONS "${PC_OGG_LINK_OPTIONS}"
    INTERFACE_COMPILE_DEFINITIONS "${PC_OGG_COMPILE_DEFINITIONS}"
    INTERFACE_COMPILE_OPTIONS "${PC_OGG_COMPILE_OPTIONS}"
    )
endif()

# EOF #
