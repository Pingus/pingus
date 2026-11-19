# Windstille - A Sci-Fi Action-Adventure Game
# Copyright (C) 2015 Ingo Ruhnke <grumbel@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.21)

# Source of truth: top-level VERSION (e.g. "0.3.0-dev"), or
# -DPROJECT_VERSION_FULL=... from packaging (Nix may append .<revCount>+g<rev>).
# A parent project's PROJECT_VERSION_FULL isn't ours when we are a subdirectory
if(NOT DEFINED PROJECT_VERSION_FULL OR NOT CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" PROJECT_VERSION_FULL LIMIT_COUNT 1)
endif()

# project() needs a numeric version; strip -dev / +git / similar suffixes.
string(REGEX MATCH "^[0-9]+(\\.[0-9]+)*" _project_version_cmake "${PROJECT_VERSION_FULL}")

project(wstsound VERSION ${_project_version_cmake} LANGUAGES CXX)

# Find modules for Opusfile/Opus/Ogg/MPG123/ModPlug/Vorbisfile (formerly from tinycmmc)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Prefer regular libraries over frameworks, so openal-soft is used
# instead of the OpenAL framework that is missing effects support.
set(CMAKE_FIND_FRAMEWORK "LAST")

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

option(BUILD_EXTRA "Build extra stuff" OFF)
option(BUILD_TESTS "Build test cases" OFF)
option(WSTSOUND_INSTALL "Generate install and export rules, off by default when wstsound is a subdirectory" ${PROJECT_IS_TOP_LEVEL})
option(WARNINGS "Enable extra compiler warnings" OFF)
option(WERROR "Treat warnings as errors" OFF)

# Codec / feature toggles. Desktop defaults keep the full set.
# Constrained targets (EMSCRIPTEN/ANDROID) disable codecs/features that are
# usually unavailable; VORBIS is left optional so projects that ship .ogg can
# keep it (pass -DWSTSOUND_WITH_VORBIS=OFF to force off).
option(WSTSOUND_WITH_MODPLUG "Decode module formats via libmodplug (.it/.s3m/…)" ON)
option(WSTSOUND_WITH_VORBIS "Decode Ogg Vorbis" ON)
option(WSTSOUND_WITH_OPUS "Decode Opus" ON)
option(WSTSOUND_WITH_MPG123 "Decode MP3 via mpg123" ON)
option(WSTSOUND_WITH_EFX "OpenAL EFX effects API (not on Emscripten OpenAL)" ON)
option(WSTSOUND_WITH_LOOPBACK "OpenAL Soft loopback device (not on Emscripten OpenAL)" ON)

if(EMSCRIPTEN OR ANDROID)
  set(WSTSOUND_WITH_MPG123 OFF CACHE BOOL "Decode MP3 via mpg123" FORCE)
  set(WSTSOUND_WITH_OPUS OFF CACHE BOOL "Decode Opus" FORCE)
  set(WSTSOUND_WITH_MODPLUG ON CACHE BOOL "Decode module formats via libmodplug" FORCE)
  set(WSTSOUND_WITH_EFX OFF CACHE BOOL "OpenAL EFX effects API" FORCE)
endif()
if(EMSCRIPTEN)
  set(WSTSOUND_WITH_LOOPBACK OFF CACHE BOOL "OpenAL Soft loopback device" FORCE)
endif()

# Compiler warnings (inlined from former tinycmmc MaximumWarnings)
set(WARNINGS_CXX_FLAGS "")
if(WERROR)
  list(APPEND WARNINGS_CXX_FLAGS -Werror)
endif()
if(WARNINGS)
  list(APPEND WARNINGS_CXX_FLAGS -Wall -Wextra)
  if(CMAKE_COMPILER_IS_GNUCXX)
    list(APPEND WARNINGS_CXX_FLAGS
      -Wcast-align
      -Wcast-qual
      -Wconversion
      -Wctor-dtor-privacy
      -Wdisabled-optimization
      -Wdouble-promotion
      -Weffc++
      -Wformat=2
      -Winit-self
      -Winvalid-pch
      -Wlogical-op
      -Wmissing-format-attribute
      -Wmissing-noreturn
      -Wno-suggest-attribute=noreturn
      -Wno-unused-parameter
      -Wnon-virtual-dtor
      -Wold-style-cast
      -Woverloaded-virtual
      -Wpacked
      -Wredundant-decls
      -Wshadow
      -Wsign-promo
      -Wstrict-null-sentinel
      -Wsuggest-override
      -Wunreachable-code
      -Wzero-as-null-pointer-constant
      -pedantic
    )
    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 8)
      list(APPEND WARNINGS_CXX_FLAGS -Wint-in-bool-context)
    endif()
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    list(APPEND WARNINGS_CXX_FLAGS
      -Weverything
      -Wno-c++98-compat
      -Wno-c++98-compat-pedantic
      -Wno-float-equal
      -Wno-padded
      -Wno-weak-vtables
      -Wno-disabled-macro-expansion
      -Wno-documentation
      -Wno-reserved-id-macro
      -Wno-sign-conversion
      -Wno-gnu-zero-variadic-macro-arguments
      -Wno-poison-system-directories
      -Wno-reserved-identifier
      -Wno-documentation-unknown-command
      -Wno-inconsistent-missing-destructor-override
      -Wno-deprecated-dynamic-exception-spec
      -Wno-deprecated
      -Wno-switch-enum
      -Wno-covered-switch-default
      -Wno-exit-time-destructors
      -Wno-global-constructors
      -Wno-duplicate-enum
      -Wno-unused-parameter
      -Wno-old-style-cast
      -Wno-unreachable-code-break
      -Wno-double-promotion
      -Wno-unused-private-field
      -Wno-unused-exception-parameter
    )
  endif()
endif()

if(EMSCRIPTEN)
  # Emscripten ships OpenAL → Web Audio; headers are on the default
  # em++ include path (sysroot). Do not find_path — the store path is
  # not always visible via ENV EMSCRIPTEN during isolated builds.
  set(OPENAL_INCLUDE_DIR "" CACHE PATH "OpenAL include (emscripten sysroot)")
  set(OPENAL_LIBRARY "openal" CACHE STRING "OpenAL link library name")
  set(OpenAL_FOUND TRUE)
  message(STATUS "Emscripten OpenAL: using toolchain sysroot, link -lopenal")
else()
  find_package(Threads REQUIRED)
  find_package(OpenAL REQUIRED)
endif()

set(WSTSOUND_CODEC_LIBS "")
if(WSTSOUND_WITH_OPUS)
  find_package(Opusfile REQUIRED)
  find_package(Opus REQUIRED)
  find_package(Ogg REQUIRED)
  list(APPEND WSTSOUND_CODEC_LIBS Opus::opus Opusfile::opusfile Ogg::ogg)
endif()
if(WSTSOUND_WITH_VORBIS)
  # Ogg may already be imported via the Opus block above.
  if(NOT TARGET Ogg::ogg)
    find_package(Ogg REQUIRED)
  endif()
  find_package(Vorbisfile REQUIRED)
  list(APPEND WSTSOUND_CODEC_LIBS Ogg::ogg Vorbisfile::vorbisfile)
endif()
if(WSTSOUND_WITH_MPG123)
  find_package(MPG123 REQUIRED)
  list(APPEND WSTSOUND_CODEC_LIBS MPG123::mpg123)
endif()
if(WSTSOUND_WITH_MODPLUG)
  find_package(ModPlug REQUIRED)
  list(APPEND WSTSOUND_CODEC_LIBS ModPlug::modplug)
endif()

file(GLOB WSTSOUND_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  include/wstsound/*.hpp)
file(GLOB WSTSOUND_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  src/*.cpp)

if(NOT WSTSOUND_WITH_MODPLUG)
  list(FILTER WSTSOUND_SOURCES EXCLUDE REGEX ".*/modplug_sound_file\\.cpp$")
endif()
if(NOT WSTSOUND_WITH_VORBIS)
  list(FILTER WSTSOUND_SOURCES EXCLUDE REGEX ".*/ogg_sound_file\\.cpp$")
endif()
if(NOT WSTSOUND_WITH_OPUS)
  list(FILTER WSTSOUND_SOURCES EXCLUDE REGEX ".*/opus_sound_file\\.cpp$")
endif()
if(NOT WSTSOUND_WITH_MPG123)
  list(FILTER WSTSOUND_SOURCES EXCLUDE REGEX ".*/mp3_sound_file\\.cpp$")
endif()
if(NOT WSTSOUND_WITH_EFX)
  list(FILTER WSTSOUND_SOURCES EXCLUDE REGEX ".*/effect(_slot)?\\.cpp$")
  list(FILTER WSTSOUND_SOURCES EXCLUDE REGEX ".*/filter\\.cpp$")
endif()
if(NOT WSTSOUND_WITH_LOOPBACK)
  list(FILTER WSTSOUND_SOURCES EXCLUDE REGEX ".*/openal_loopback_device\\.cpp$")
endif()

add_library(wstsound STATIC ${WSTSOUND_SOURCES})
add_library(wstsound::wstsound ALIAS wstsound)
set_target_properties(wstsound PROPERTIES PUBLIC_HEADER "${WSTSOUND_HEADERS}")
target_compile_options(wstsound PRIVATE ${WARNINGS_CXX_FLAGS})
set_target_properties(wstsound PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF)

if(WSTSOUND_WITH_MODPLUG)
  target_compile_definitions(wstsound PUBLIC WSTSOUND_WITH_MODPLUG=1)
endif()
if(WSTSOUND_WITH_VORBIS)
  target_compile_definitions(wstsound PUBLIC WSTSOUND_WITH_VORBIS=1)
endif()
if(WSTSOUND_WITH_OPUS)
  target_compile_definitions(wstsound PUBLIC WSTSOUND_WITH_OPUS=1)
endif()
if(WSTSOUND_WITH_MPG123)
  target_compile_definitions(wstsound PUBLIC WSTSOUND_WITH_MPG123=1)
endif()
if(WSTSOUND_WITH_EFX)
  target_compile_definitions(wstsound PUBLIC WSTSOUND_WITH_EFX=1)
endif()
if(WSTSOUND_WITH_LOOPBACK)
  target_compile_definitions(wstsound PUBLIC WSTSOUND_WITH_LOOPBACK=1)
endif()

if(OPENAL_INCLUDE_DIR)
  target_include_directories(wstsound SYSTEM PUBLIC ${OPENAL_INCLUDE_DIR})
endif()
target_include_directories(wstsound PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  )
target_include_directories(wstsound PRIVATE src/ include/wstsound/)
target_link_libraries(wstsound PUBLIC ${OPENAL_LIBRARY} ${WSTSOUND_CODEC_LIBS})
if(NOT EMSCRIPTEN)
  target_link_libraries(wstsound PUBLIC Threads::Threads)
endif()

if(BUILD_TESTS)
  find_package(GTest REQUIRED)

  file(GLOB TEST_WSTSOUND_SOURCES test/*_test.cpp)
  add_executable(test_wstsound ${TEST_WSTSOUND_SOURCES})
  target_include_directories(test_wstsound PUBLIC src/ include/wstsound/)
  target_compile_options(test_wstsound PRIVATE ${WARNINGS_CXX_FLAGS})
  set_target_properties(test_wstsound PROPERTIES
      CXX_STANDARD 20
      CXX_STANDARD_REQUIRED ON
      CXX_EXTENSIONS OFF)
  target_link_libraries(test_wstsound
    GTest::GTest
    GTest::Main
    wstsound)

  enable_testing()
  add_test(NAME test_wstsound
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test/
    COMMAND test_wstsound)
  if(BUILD_EXTRA)
    add_test(NAME openal_info
      COMMAND openal_info)
  endif()
endif()

if(BUILD_EXTRA)
  file(GLOB EXTRA_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    extra/*.cpp)
  foreach(SRC ${EXTRA_SOURCES})
    get_filename_component(SRC_BASENAME ${SRC} NAME_WE)
    add_executable(${SRC_BASENAME} ${SRC})
    target_include_directories(${SRC_BASENAME} PUBLIC src/)
    target_link_libraries(${SRC_BASENAME} wstsound)
    set_target_properties(${SRC_BASENAME} PROPERTIES
      CXX_STANDARD 20
      CXX_STANDARD_REQUIRED ON
      CXX_EXTENSIONS OFF)
    if(WSTSOUND_INSTALL)
      install(TARGETS ${SRC_BASENAME}
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
    endif()
  endforeach(SRC)
endif()

# Emit only the find_dependency() calls that match the codecs this
# build actually linked. Consumers (e.g. Pingus MinGW) must not be forced
# to provide Opus/Vorbis/mpg123 when wstsound was built without them.
set(WSTSOUND_FIND_DEPENDENCY_CODE "")
if(WSTSOUND_WITH_OPUS)
  string(APPEND WSTSOUND_FIND_DEPENDENCY_CODE
    "find_dependency(Opusfile REQUIRED)\n"
    "find_dependency(Opus REQUIRED)\n"
    "find_dependency(Ogg REQUIRED)\n")
endif()
if(WSTSOUND_WITH_VORBIS)
  # Ogg may already be listed via the Opus block above.
  if(NOT WSTSOUND_WITH_OPUS)
    string(APPEND WSTSOUND_FIND_DEPENDENCY_CODE
      "find_dependency(Ogg REQUIRED)\n")
  endif()
  string(APPEND WSTSOUND_FIND_DEPENDENCY_CODE
    "find_dependency(Vorbisfile REQUIRED)\n")
endif()
if(WSTSOUND_WITH_MPG123)
  string(APPEND WSTSOUND_FIND_DEPENDENCY_CODE
    "find_dependency(MPG123 REQUIRED)\n")
endif()
if(WSTSOUND_WITH_MODPLUG)
  string(APPEND WSTSOUND_FIND_DEPENDENCY_CODE
    "find_dependency(ModPlug REQUIRED)\n")
endif()

if(WSTSOUND_INSTALL)
  # Export and install library (inlined from former tinycmmc)
  install(TARGETS wstsound
    EXPORT wstsound
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/wstsound")

  # # Substitute codec-specific find_dependency lines into the config file
  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/wstsound-config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/wstsound-config.cmake.in.tmp"
    @ONLY)
  configure_package_config_file(
    "${CMAKE_CURRENT_BINARY_DIR}/wstsound-config.cmake.in.tmp"
    "${CMAKE_CURRENT_BINARY_DIR}/wstsound-config.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/wstsound")
  write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/wstsound-config-version.cmake"
    VERSION "${PROJECT_VERSION}"
    COMPATIBILITY SameMinorVersion)
  export(EXPORT wstsound
    NAMESPACE wstsound::
    FILE "${CMAKE_CURRENT_BINARY_DIR}/wstsound-targets.cmake")
  install(EXPORT wstsound
    FILE wstsound-targets.cmake
    NAMESPACE wstsound::
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/wstsound")
  install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/wstsound-config-version.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/wstsound-config.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/wstsound")

  # Ship Find modules so consumers can resolve Opusfile/Opus/Ogg/etc.
  install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/cmake/"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/wstsound"
    FILES_MATCHING PATTERN "Find*.cmake")
endif()

# EOF #
