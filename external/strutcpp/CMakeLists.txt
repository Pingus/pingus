# strut - Collection of string utilities
# Copyright (C) 2020 Ingo Ruhnke <grumbel@gmail.com>
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

cmake_minimum_required(VERSION 3.6)
project(strut VERSION 0.0.0)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

option(BUILD_TESTS "Build tests" OFF)
option(WARNINGS "Enable extra compiler warnings" OFF)
option(WERROR "Treat warnings as errors" OFF)
option(CLANG_TIDY "Enable clang-tidy" OFF)
option(CLANG_TIDY_FIX "Apply clang-tidy fixes to source code" OFF)

set(WARNINGS_CXX_FLAGS "")
if(WERROR)
  list(APPEND WARNINGS_CXX_FLAGS -Werror)
endif()
if(WARNINGS)
  list(APPEND WARNINGS_CXX_FLAGS -Wall -Wextra -Wpedantic)
endif()

if(CLANG_TIDY AND CMAKE_VERSION VERSION_GREATER_EQUAL 3.6)
  find_program(CLANG_TIDY_EXE NAMES clang-tidy DOC "Path to clang-tidy executable")
  if(NOT CLANG_TIDY_EXE)
    message(FATAL_ERROR "clang-tidy not found.")
  endif()
  message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
  if(CLANG_TIDY_FIX)
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}" "-fix")
  else()
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
  endif()
endif()

file(GLOB STRUT_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  include/strut/*.hpp)

file(GLOB STRUT_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  src/*.cpp)

add_library(strut STATIC ${STRUT_SOURCES})
set_target_properties(strut PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
  PUBLIC_HEADER "${STRUT_HEADERS}"
  EXPORT_NAME strut)
target_include_directories(strut INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_include_directories(strut PRIVATE include/strut/)
target_compile_options(strut PRIVATE ${WARNINGS_CXX_FLAGS})

add_library(strut::strut ALIAS strut)

if(BUILD_TESTS)
  find_package(GTest REQUIRED)

  file(GLOB TEST_STRUT_SOURCES test/*_test.cpp)
  add_executable(test_strut ${TEST_STRUT_SOURCES})
  set_target_properties(test_strut PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF)
  target_compile_options(test_strut PRIVATE ${WARNINGS_CXX_FLAGS})
  target_link_libraries(test_strut
    strut
    GTest::GTest
    GTest::Main)

  enable_testing()
  add_test(NAME test_strut
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    COMMAND test_strut)
endif()

install(TARGETS strut
  EXPORT strut
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/strut)

configure_package_config_file(
  strut-config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/strut-config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/strut)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/strut-config-version.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMinorVersion)

export(EXPORT strut
  NAMESPACE strut::
  FILE ${CMAKE_CURRENT_BINARY_DIR}/strut-targets.cmake)

install(EXPORT strut
  FILE strut-targets.cmake
  NAMESPACE strut::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/strut)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/strut-config.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/strut-config-version.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/strut)

# EOF #
