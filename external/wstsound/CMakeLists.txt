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

cmake_minimum_required(VERSION 3.15)
project(wstsound VERSION 0.3.0)

# Prefer regular libraries over frameworks, so openal-soft is used
# instead of the OpenAL framework that is missing effects support.
set(CMAKE_FIND_FRAMEWORK "LAST")

if(NOT tinycmmc_FOUND)
  find_package(tinycmmc CONFIG QUIET)
endif()
if(tinycmmc_FOUND AND TINYCMMC_MODULE_PATH)
  # use package
elseif(TINYCMMC_MODULE_PATH AND EXISTS "${TINYCMMC_MODULE_PATH}/TinyCMMC.cmake")
  # Caller passed -DTINYCMMC_MODULE_PATH=... (e.g. isolated wasm package)
elseif(DEFINED ENV{TINYCMMC_MODULE_PATH} AND EXISTS "$ENV{TINYCMMC_MODULE_PATH}/TinyCMMC.cmake")
  set(TINYCMMC_MODULE_PATH "$ENV{TINYCMMC_MODULE_PATH}")
elseif(EXISTS "${CMAKE_SOURCE_DIR}/external/tinycmmc/modules/TinyCMMC.cmake")
  set(TINYCMMC_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/tinycmmc/modules/")
elseif(EXISTS "${CMAKE_CURRENT_LIST_DIR}/external/tinycmmc/modules/TinyCMMC.cmake")
  set(TINYCMMC_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/external/tinycmmc/modules/")
elseif(EXISTS "${CMAKE_CURRENT_LIST_DIR}/external/cmake-modules/modules/TinyCMMC.cmake")
  set(TINYCMMC_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/external/cmake-modules/modules/")
elseif(EXISTS "${CMAKE_SOURCE_DIR}/../tinycmmc/modules/TinyCMMC.cmake")
  # monorepo: external/wstsound next to external/tinycmmc
  set(TINYCMMC_MODULE_PATH "${CMAKE_SOURCE_DIR}/../tinycmmc/modules/")
else()
  message(FATAL_ERROR "tinycmmc modules not found (set TINYCMMC_MODULE_PATH)")
endif()
message(STATUS "tinycmmc module path: ${TINYCMMC_MODULE_PATH}")
list(APPEND CMAKE_MODULE_PATH ${TINYCMMC_MODULE_PATH})

include(GNUInstallDirs)
include(ClangTidy)
include(MaximumWarnings)

option(BUILD_EXTRA "Build extra stuff" OFF)

# Codec / feature toggles. Desktop defaults keep the full set; Emscripten
# defaults match Pingus in-tree assets (WAV + module music via ModPlug).
option(WSTSOUND_WITH_MODPLUG "Decode module formats via libmodplug (.it/.s3m/…)" ON)
option(WSTSOUND_WITH_VORBIS "Decode Ogg Vorbis" ON)
option(WSTSOUND_WITH_OPUS "Decode Opus" ON)
option(WSTSOUND_WITH_MPG123 "Decode MP3 via mpg123" ON)
option(WSTSOUND_WITH_EFX "OpenAL EFX effects API (not on Emscripten OpenAL)" ON)

# Mobile / wasm: stock Pingus data is WAV SFX + .it/.s3m modules only.
if(EMSCRIPTEN OR ANDROID)
  set(WSTSOUND_WITH_MPG123 OFF CACHE BOOL "Decode MP3 via mpg123" FORCE)
  set(WSTSOUND_WITH_VORBIS OFF CACHE BOOL "Decode Ogg Vorbis" FORCE)
  set(WSTSOUND_WITH_OPUS OFF CACHE BOOL "Decode Opus" FORCE)
  set(WSTSOUND_WITH_MODPLUG ON CACHE BOOL "Decode module formats via libmodplug" FORCE)
  set(WSTSOUND_WITH_EFX OFF CACHE BOOL "OpenAL EFX effects API" FORCE)
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
if(EMSCRIPTEN)
  # Soft-only loopback device API
  list(FILTER WSTSOUND_SOURCES EXCLUDE REGEX ".*/openal_loopback_device\\.cpp$")
endif()

add_library(wstsound STATIC ${WSTSOUND_SOURCES})
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
    install(TARGETS ${SRC_BASENAME}
      RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
  endforeach(SRC)
endif()

include(ExportAndInstallLibrary)

# EOF #
