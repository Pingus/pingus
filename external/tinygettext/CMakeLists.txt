# tinygettext - A gettext replacement that works directly on .po files
# Copyright (C) 2006 Christoph Sommer <christoph.sommer@2006.expires.deltadevelopment.de>,
#               2020 Ingo Ruhnke <grumbel@gmail.com>
#
# This software is provided 'as-is', without any express or implied
# warranty. In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented; you must not
#    claim that you wrote the original software. If you use this software
#    in a product, an acknowledgement in the product documentation would be
#    appreciated but is not required.
# 2. Altered source versions must be plainly marked as such, and must not be
#    misrepresented as being the original software.
# 3. This notice may not be removed or altered from any source distribution.

cmake_minimum_required(VERSION 3.10)
project(tinygettext VERSION "0.2.0")

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

option(BUILD_TESTS "Build test cases" OFF)
option(WARNINGS "Enable extra compiler warnings" OFF)
option(WERROR "Treat warnings as errors" OFF)
option(CLANG_TIDY "Enable clang-tidy" OFF)
option(CLANG_TIDY_FIX "Apply clang-tidy fixes to source code" OFF)
option(TINYGETTEXT_UTF8_ONLY "Build without iconv, UTF-8 only" OFF)
option(TINYGETTEXT_WITH_SDL "Use SDL2 for iconv instead of libiconv" OFF)

# Optional extra warnings (replaces tinycmmc MaximumWarnings)
set(WARNINGS_CXX_FLAGS "")
if(WERROR)
  list(APPEND WARNINGS_CXX_FLAGS -Werror)
endif()
if(WARNINGS)
  list(APPEND WARNINGS_CXX_FLAGS -Wall -Wextra -Wpedantic)
endif()

# Optional clang-tidy (replaces tinycmmc ClangTidy)
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

file(GLOB TINYGETTEXT_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp)
file(GLOB TINYGETTEXT_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} include/tinygettext/*.hpp)

add_library(tinygettext STATIC ${TINYGETTEXT_SOURCES})
set_target_properties(tinygettext PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
  PUBLIC_HEADER "${TINYGETTEXT_HEADERS}"
  EXPORT_NAME tinygettext)
target_include_directories(tinygettext PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_compile_options(tinygettext PRIVATE ${WARNINGS_CXX_FLAGS})
target_compile_definitions(tinygettext PRIVATE VERSION=${PROJECT_VERSION})

add_library(tinygettext::tinygettext ALIAS tinygettext)

if(TINYGETTEXT_UTF8_ONLY)
  target_compile_definitions(tinygettext PUBLIC TINYGETTEXT_UTF8_ONLY)
elseif(TINYGETTEXT_WITH_SDL)
  find_package(PkgConfig REQUIRED)
  pkg_search_module(SDL2 REQUIRED sdl2 IMPORTED_TARGET)
  target_compile_definitions(tinygettext PUBLIC TINYGETTEXT_WITH_SDL)
  target_link_libraries(tinygettext PUBLIC PkgConfig::SDL2)
else()
  find_package(Iconv REQUIRED)
  target_link_libraries(tinygettext PUBLIC Iconv::Iconv)
endif()

if(WIN32)
  target_compile_definitions(tinygettext PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

# pkg-config
set(LIB_SUBDIR ${CMAKE_INSTALL_LIBDIR})
configure_file(tinygettext.pc.in tinygettext.pc @ONLY)

install(TARGETS tinygettext
  EXPORT tinygettext
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/tinygettext.pc
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

# CMake package config (replaces tinycmmc_export_and_install_library)
configure_package_config_file(
  tinygettext-config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/tinygettext-config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tinygettext)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/tinygettext-config-version.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMinorVersion)

export(EXPORT tinygettext
  NAMESPACE tinygettext::
  FILE ${CMAKE_CURRENT_BINARY_DIR}/tinygettext-targets.cmake)

install(EXPORT tinygettext
  FILE tinygettext-targets.cmake
  NAMESPACE tinygettext::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tinygettext)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/tinygettext-config.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/tinygettext-config-version.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tinygettext)

if(BUILD_TESTS)
  foreach(TEST tinygettext_test po_parser_test)
    add_executable(${TEST} test/${TEST}.cpp)
    set_target_properties(${TEST} PROPERTIES
      CXX_STANDARD 17
      CXX_STANDARD_REQUIRED ON
      CXX_EXTENSIONS OFF)
    target_compile_options(${TEST} PRIVATE ${WARNINGS_CXX_FLAGS})
    if(WIN32)
      target_compile_definitions(${TEST} PRIVATE _CRT_SECURE_NO_WARNINGS)
    endif()
    target_link_libraries(${TEST} PRIVATE tinygettext)
  endforeach(TEST)
endif()

# EOF #
