# SExp - A S-Expression Parser for C++
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

cmake_minimum_required(VERSION 3.21...4.1)

# Source of truth: top-level VERSION file (e.g. "0.2.0-dev"), or
# -DPROJECT_VERSION_FULL=... from packaging (Nix may append .<revCount>+g<rev>).
# A parent project's PROJECT_VERSION_FULL isn't ours when we are a subdirectory
if(NOT DEFINED PROJECT_VERSION_FULL OR NOT CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" PROJECT_VERSION_FULL LIMIT_COUNT 1)
endif()

# CMake project() VERSION expects major[.minor[.patch[.tweak]]]; strip
# any -dev / +git suffix for the numeric project version.
string(REGEX MATCH "^[0-9]+(\\.[0-9]+)*" _project_version_cmake "${PROJECT_VERSION_FULL}")

project(sexp VERSION ${_project_version_cmake} LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(CheckCXXSourceCompiles)

option(BUILD_TESTS "Build tests" OFF)
option(BUILD_BENCHMARKS "Build benchmarks" OFF)
option(SEXP_INSTALL "Generate install and export rules, off by default when sexp-cpp is a subdirectory" ${PROJECT_IS_TOP_LEVEL})
option(WARNINGS "Enable extra compiler warnings" OFF)
option(WERROR "Treat warnings as errors" OFF)
option(CLANG_TIDY "Enable clang-tidy" OFF)
option(CLANG_TIDY_FIX "Apply clang-tidy fixes to source code" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# std::from_chars() and std::to_chars() for float are missing from older
# standard libraries (libstdc++ before GCC 11, from_chars() in libc++
# before LLVM 20). They can also be declared in the headers but missing
# from an older libstdc++.so the program is linked against (R36S), so
# check by linking, not just by compiling. Without them src/float.cpp
# falls back to iostreams with the classic locale.
check_cxx_source_compiles("
#include <charconv>
int main(int argc, char** argv)
{
  float f = 0.0f;
  double d = 0.0;
  auto const rf = std::from_chars(argv[0], argv[0] + argc, f);
  auto const rd = std::from_chars(argv[0], argv[0] + argc, d);
  return static_cast<int>(rf.ec) + static_cast<int>(rd.ec) + static_cast<int>(f + static_cast<float>(d));
}" SEXP_HAVE_FROM_CHARS_FLOAT)

check_cxx_source_compiles("
#include <charconv>
int main(int argc, char** argv)
{
  auto const result = std::to_chars(argv[0], argv[0] + argc, static_cast<float>(argc));
  return static_cast<int>(result.ec);
}" SEXP_HAVE_TO_CHARS_FLOAT)

set(WARNINGS_CXX_FLAGS "")
if(WERROR)
  list(APPEND WARNINGS_CXX_FLAGS -Werror)
endif()
if(WARNINGS)
  list(APPEND WARNINGS_CXX_FLAGS -Wall -Wextra -Wpedantic
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
      -Wunused-parameter
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
    )
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

file(GLOB SEXP_SOURCES src/*.cpp)
file(GLOB SEXP_HEADER_SOURCES include/sexp/*.hpp)
add_library(sexp STATIC ${SEXP_SOURCES})
set_target_properties(sexp PROPERTIES
  PUBLIC_HEADER "${SEXP_HEADER_SOURCES}"
  EXPORT_NAME sexp)
target_compile_options(sexp PRIVATE ${WARNINGS_CXX_FLAGS})
target_compile_definitions(sexp PRIVATE
  $<$<BOOL:${SEXP_HAVE_FROM_CHARS_FLOAT}>:SEXP_HAVE_FROM_CHARS_FLOAT>
  $<$<BOOL:${SEXP_HAVE_TO_CHARS_FLOAT}>:SEXP_HAVE_TO_CHARS_FLOAT>)
target_include_directories(sexp SYSTEM PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

add_library(sexp::sexp ALIAS sexp)

if(SEXP_INSTALL)
  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/pkgconfig/sexp.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/pkgconfig/sexp.pc"
    @ONLY)

  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/pkgconfig/sexp.pc"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")

  install(TARGETS sexp
    EXPORT sexp
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/sexp)

  configure_package_config_file(
    sexp-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/sexp-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/sexp)

  write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/sexp-config-version.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMinorVersion)

  export(EXPORT sexp
    NAMESPACE sexp::
    FILE ${CMAKE_CURRENT_BINARY_DIR}/sexp-targets.cmake)

  install(EXPORT sexp
    FILE sexp-targets.cmake
    NAMESPACE sexp::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/sexp)

  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/sexp-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/sexp-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/sexp)
endif()

if(BUILD_TESTS)
  find_package(GTest REQUIRED)

  # build sexp tests
  file(GLOB TEST_SEXP_SOURCES tests/*.cpp)
  add_executable(test_sexp ${TEST_SEXP_SOURCES})
  target_compile_options(test_sexp PRIVATE ${WARNINGS_CXX_FLAGS})
  target_include_directories(test_sexp PUBLIC src/)
  target_link_libraries(test_sexp
    GTest::GTest
    GTest::Main
    sexp)

  # add 'make test' target, use 'make test ARGS="-V"' or 'ctest -V' for verbose
  enable_testing()
  add_test(NAME test_sexp
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND test_sexp)
endif()

if(BUILD_BENCHMARKS)
  find_package(benchmark REQUIRED)
  find_package(Threads REQUIRED)

  # build benchmarks
  file(GLOB BENCHMARKSOURCES benchmarks/*.cpp)
  foreach(SOURCE ${BENCHMARKSOURCES})
    get_filename_component(SOURCE_BASENAME ${SOURCE} NAME_WE)
    add_executable(${SOURCE_BASENAME} ${SOURCE})
    target_link_libraries(${SOURCE_BASENAME}
      sexp
      benchmark::benchmark
      Threads::Threads
      )
    set_target_properties(${SOURCE_BASENAME} PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/benchmarks/")
    target_compile_options(${SOURCE_BASENAME} PRIVATE ${WARNINGS_CXX_FLAGS})
  endforeach()
endif()

# EOF #
