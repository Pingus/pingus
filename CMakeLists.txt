# geom - Basic collection of point, size and rect classes
# Copyright (C) 2020 Ingo Ruhnke <grumbel@gmail.com>
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

cmake_minimum_required(VERSION 3.10...4.1)

# Source of truth: top-level VERSION file (e.g. "0.1.0-dev"), or
# -DPROJECT_VERSION_FULL=... from packaging (Nix may append .<revCount>+g<rev>).
if(NOT DEFINED PROJECT_VERSION_FULL)
  file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" PROJECT_VERSION_FULL LIMIT_COUNT 1)
endif()

# CMake project() VERSION expects major[.minor[.patch[.tweak]]]; strip
# any -dev / +git suffix for the numeric project version.
string(REGEX MATCH "^[0-9]+(\\.[0-9]+)*" _project_version_cmake "${PROJECT_VERSION_FULL}")

project(geom VERSION ${_project_version_cmake} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

option(BUILD_TESTS "Build test cases" OFF)
option(WARNINGS "Switch on extra warnings" OFF)
option(WERROR "Turn warnings into errors" OFF)

set(GEOM_WARNINGS_CXX_FLAGS "")
if(WERROR)
  list(APPEND GEOM_WARNINGS_CXX_FLAGS -Werror)
endif()
if(WARNINGS)
  list(APPEND GEOM_WARNINGS_CXX_FLAGS -Wall -Wextra -Wpedantic)
endif()

# Must guard find_package(glm) as otherwise error will result
if(NOT TARGET glm::glm)
  find_package(glm REQUIRED)
endif()

file(GLOB GEOM_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  include/geom/*.hpp)

add_library(geom INTERFACE)
target_include_directories(geom INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_link_libraries(geom INTERFACE glm::glm)
target_compile_definitions(geom INTERFACE -DGLM_ENABLE_EXPERIMENTAL)
set_target_properties(geom PROPERTIES PUBLIC_HEADER
  "${GEOM_HEADERS}")

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/geom.pc.in"
  "${CMAKE_CURRENT_BINARY_DIR}/geom.pc"
  @ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/geom.pc"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")

if(BUILD_TESTS)
  enable_testing()
  find_package(GTest REQUIRED)

  file(GLOB TEST_GEOM_SOURCES test/*_test.cpp)
  add_executable(test_geom ${TEST_GEOM_SOURCES})
  target_compile_options(test_geom PRIVATE ${GEOM_WARNINGS_CXX_FLAGS})
  target_link_libraries(test_geom
    GTest::GTest
    GTest::Main
    geom)

  add_test(NAME test_geom
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND test_geom)
endif()

# Export and install (inlined from former tinycmmc helper)
install(TARGETS geom
  EXPORT geom
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/geom")

add_library(geom::geom ALIAS geom)

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/geom-config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/geom-config.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/geom")

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/geom-config-version.cmake"
  VERSION "${PROJECT_VERSION}"
  COMPATIBILITY SameMinorVersion)

export(EXPORT geom
  NAMESPACE geom::
  FILE "${CMAKE_CURRENT_BINARY_DIR}/geom-targets.cmake")

install(EXPORT geom
  FILE geom-targets.cmake
  NAMESPACE geom::
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/geom")

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/geom-config-version.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/geom-config.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/geom")

# EOF #
