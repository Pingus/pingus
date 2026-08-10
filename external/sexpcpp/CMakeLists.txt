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

cmake_minimum_required(VERSION 3.10)
project(sexp VERSION 0.1.0)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

option(BUILD_TESTS "Build tests" OFF)
option(BUILD_BENCHMARKS "Build benchmarks" OFF)
option(WARNINGS "Enable extra compiler warnings" OFF)
option(WERROR "Treat warnings as errors" OFF)
option(CLANG_TIDY "Enable clang-tidy" OFF)
option(CLANG_TIDY_FIX "Apply clang-tidy fixes to source code" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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

file(GLOB SEXP_SOURCES src/*.cpp)
file(GLOB SEXP_HEADER_SOURCES include/sexp/*.hpp)
add_library(sexp STATIC ${SEXP_SOURCES})
set_target_properties(sexp PROPERTIES
  PUBLIC_HEADER "${SEXP_HEADER_SOURCES}"
  EXPORT_NAME sexp)
target_compile_options(sexp PRIVATE ${WARNINGS_CXX_FLAGS})
target_include_directories(sexp SYSTEM PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

add_library(sexp::sexp ALIAS sexp)

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
