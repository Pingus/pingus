# Copyright (C) 2022 Ingo Ruhnke <grumbel@gmail.com>
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

# Map CMake package names to external/ directory names when they differ
# (repo name vs project() name).
function(_tinycmmc_external_dir _NAME _outvar)
  set(_dir "${CMAKE_SOURCE_DIR}/external/${_NAME}")
  if(EXISTS "${_dir}/CMakeLists.txt")
    set(${_outvar} "${_dir}" PARENT_SCOPE)
    return()
  endif()

  # Common repo-name aliases used by Pingus and related projects
  if(_NAME STREQUAL "geom")
    set(_alt "geomcpp")
  elseif(_NAME STREQUAL "prio")
    set(_alt "priocpp")
  elseif(_NAME STREQUAL "strut")
    set(_alt "strutcpp")
  elseif(_NAME STREQUAL "sexp")
    set(_alt "sexpcpp")
  else()
    set(_alt "")
  endif()

  if(_alt)
    set(_dir "${CMAKE_SOURCE_DIR}/external/${_alt}")
    if(EXISTS "${_dir}/CMakeLists.txt")
      set(${_outvar} "${_dir}" PARENT_SCOPE)
      return()
    endif()
  endif()

  # Legacy: relative to the calling project (submodule layout)
  set(_dir "${CMAKE_CURRENT_SOURCE_DIR}/external/${_NAME}")
  if(EXISTS "${_dir}/CMakeLists.txt")
    set(${_outvar} "${_dir}" PARENT_SCOPE)
    return()
  endif()

  set(${_outvar} "" PARENT_SCOPE)
endfunction()

macro(tinycmmc_find_dependency _NAME)
  find_package(${_NAME} QUIET)
  if(${${_NAME}_FOUND})
    message(STATUS "Found ${_NAME}: ${${_NAME}_DIR}")
  else()
    message(STATUS "Package ${_NAME} not found, trying external/")

    _tinycmmc_external_dir("${_NAME}" _tinycmmc_dep_dir)
    if(_tinycmmc_dep_dir STREQUAL "")
      message(FATAL_ERROR
        "Could not find ${_NAME} (no find_package result and no external/ sources).\n"
        "Expected external/${_NAME} (or a known alias) under the top-level source tree.")
    else()
      set(BUILD_TESTS OFF)
      add_subdirectory("${_tinycmmc_dep_dir}" "${CMAKE_BINARY_DIR}/external_${_NAME}" EXCLUDE_FROM_ALL)
      message(STATUS "Found ${_NAME}: ${_tinycmmc_dep_dir}")
    endif()
  endif()
endmacro()

# EOF #
