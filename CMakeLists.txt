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

set(TINYCMMC_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/external/tinycmmc/modules/")
find_package(tinycmmc CONFIG)
message(STATUS "tinycmmc module path: ${TINYCMMC_MODULE_PATH}")
list(APPEND CMAKE_MODULE_PATH ${TINYCMMC_MODULE_PATH})

# include(GetProjectVersion)
include(GNUInstallDirs)
include(ClangTidy)
include(MaximumWarnings)

option(BUILD_EXTRA "Build extra stuff" OFF)

find_package(Threads REQUIRED)
find_package(OpenAL REQUIRED)
find_package(Opusfile REQUIRED)
find_package(Opus REQUIRED)
find_package(Ogg REQUIRED)
find_package(Vorbisfile REQUIRED)
find_package(MPG123 REQUIRED)
find_package(ModPlug REQUIRED)

file(GLOB WSTSOUND_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  include/wstsound/*.hpp)
file(GLOB WSTSOUND_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  src/*.cpp)
add_library(wstsound STATIC ${WSTSOUND_SOURCES})
set_target_properties(wstsound PROPERTIES PUBLIC_HEADER "${WSTSOUND_HEADERS}")
target_compile_options(wstsound PRIVATE ${WARNINGS_CXX_FLAGS})
set_target_properties(wstsound PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF)
target_include_directories(wstsound SYSTEM PUBLIC ${OPENAL_INCLUDE_DIR})
target_include_directories(wstsound PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  )
target_include_directories(wstsound PRIVATE src/ include/wstsound/)
target_link_libraries(wstsound PUBLIC
  ${OPENAL_LIBRARY}
  Ogg::ogg
  Vorbisfile::vorbisfile
  Opus::opus
  Opusfile::opusfile
  MPG123::mpg123
  ModPlug::modplug
  Threads::Threads)

if(BUILD_TESTS)
  find_package(GTest REQUIRED)

  # build automatic tests
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
