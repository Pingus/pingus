# LogMich - A Trivial Logging Library
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

cmake_minimum_required(VERSION 3.10)
project(logmich VERSION 0.2.0)

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

set(LOGMICH_HEADERS
  include/logmich/logger.hpp
  include/logmich/log.hpp)
add_library(logmich STATIC
  src/logger.cpp
  src/log.cpp)
set_target_properties(logmich PROPERTIES
  PUBLIC_HEADER "${LOGMICH_HEADERS}"
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
  EXPORT_NAME logmich)
target_compile_options(logmich PUBLIC ${WARNINGS_CXX_FLAGS})
target_include_directories(logmich PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

add_library(logmich::logmich ALIAS logmich)

if(BUILD_TESTS)
  add_executable(test_logmich test/test_logmich.cpp)
  set_target_properties(test_logmich PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF)
  target_link_libraries(test_logmich logmich)

  enable_testing()
  add_test(NAME test_logmich
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND
    ${CMAKE_CURRENT_SOURCE_DIR}/test/test_logmich.sh
    ${CMAKE_CURRENT_BINARY_DIR}/test_logmich
    ${CMAKE_CURRENT_SOURCE_DIR}/test/test_logmich.expected
    ${CMAKE_CURRENT_SOURCE_DIR})
endif()

install(TARGETS logmich
  EXPORT logmich
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/logmich)

configure_package_config_file(
  logmich-config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/logmich-config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/logmich)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/logmich-config-version.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMinorVersion)

export(EXPORT logmich
  NAMESPACE logmich::
  FILE ${CMAKE_CURRENT_BINARY_DIR}/logmich-targets.cmake)

install(EXPORT logmich
  FILE logmich-targets.cmake
  NAMESPACE logmich::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/logmich)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/logmich-config.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/logmich-config-version.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/logmich)

# EOF #
