# prio - Property I/O for C++
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

cmake_minimum_required(VERSION 3.21...4.1)

# Source of truth: top-level VERSION file (e.g. "0.1.0-dev"), or
# -DPROJECT_VERSION_FULL=... from packaging (Nix may append .<revCount>+g<rev>).
# A parent project's PROJECT_VERSION_FULL isn't ours when we are a subdirectory
if(NOT DEFINED PROJECT_VERSION_FULL OR NOT CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" PROJECT_VERSION_FULL LIMIT_COUNT 1)
endif()

# CMake project() VERSION expects major[.minor[.patch[.tweak]]]; strip
# any -dev / +git suffix for the numeric project version.
string(REGEX MATCH "^[0-9]+(\\.[0-9]+)*" _project_version_cmake "${PROJECT_VERSION_FULL}")

project(prio VERSION ${_project_version_cmake} LANGUAGES CXX)

message(STATUS "Project Name: ${PROJECT_NAME}")
message(STATUS "Project Version: ${PROJECT_VERSION}")
message(STATUS "Project Version Full: ${PROJECT_VERSION_FULL}")

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_TESTS "Build test cases" OFF)
option(BUILD_EXTRA "Build extra tools (priotool)" OFF)
option(PRIO_INSTALL "Generate install and export rules, off by default when prio is a subdirectory" ${PROJECT_IS_TOP_LEVEL})
option(PRIO_USE_JSONCPP "Enable support for jsoncpp" ON)
option(PRIO_USE_SEXPCPP "Enable support for sexp-cpp" ON)
option(WARNINGS "Enable extra compiler warnings" OFF)
option(WERROR "Treat warnings as errors" OFF)

if(NOT PRIO_USE_JSONCPP AND NOT PRIO_USE_SEXPCPP)
  message(FATAL_ERROR "At least one of PRIO_USE_JSONCPP or PRIO_USE_SEXPCPP must be ON")
endif()

# --- compiler warnings (inlined from former tinycmmc MaximumWarnings) ---
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
      -Wsuggest-attribute=noreturn
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

find_package(PkgConfig REQUIRED)

if(PRIO_USE_JSONCPP)
  pkg_search_module(JSONCPP REQUIRED jsoncpp IMPORTED_TARGET)
endif()

# The targets already exist when prio is a subdirectory next to them
if(PRIO_USE_SEXPCPP AND NOT TARGET sexp::sexp)
  find_package(sexp REQUIRED)
endif()
if(NOT TARGET logmich::logmich)
  find_package(logmich REQUIRED)
endif()

set(PRIO_HEADERS
  include/prio/error_handler.hpp
  include/prio/format.hpp
  include/prio/format_util.hpp
  include/prio/fwd.hpp
  include/prio/override_reader_mapping.hpp
  include/prio/prio.hpp
  include/prio/reader_collection.hpp
  include/prio/reader_document.hpp
  include/prio/reader_error.hpp
  include/prio/reader.hpp
  include/prio/reader_impl.hpp
  include/prio/reader_mapping.hpp
  include/prio/reader_object.hpp
  include/prio/writer.hpp)

set(PRIO_SOURCES
  src/float_format.cpp
  src/override_reader_mapping.cpp
  src/reader_collection.cpp
  src/reader_document.cpp
  src/reader_error.cpp
  src/reader_mapping.cpp
  src/reader_object.cpp
  src/writer.cpp)

if(PRIO_USE_JSONCPP)
  list(APPEND PRIO_HEADERS
    include/prio/json_reader_impl.hpp)

  list(APPEND PRIO_SOURCES
    src/jsonpretty_writer_impl.cpp
    src/json_reader_impl.cpp
    src/json_writer_impl.cpp)
endif()

if(PRIO_USE_SEXPCPP)
  list(APPEND PRIO_HEADERS
    include/prio/sexpr_reader_impl.hpp)

  list(APPEND PRIO_SOURCES
    src/sexpr_reader_impl.cpp
    src/sexpr_writer_impl.cpp)
endif()

add_library(prio STATIC ${PRIO_SOURCES})
add_library(prio::prio ALIAS prio)

set_target_properties(prio PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
  PUBLIC_HEADER "${PRIO_HEADERS}")

target_compile_options(prio PRIVATE ${WARNINGS_CXX_FLAGS})
target_include_directories(prio
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  PRIVATE
    src/
    include/prio/)
target_link_libraries(prio PUBLIC logmich::logmich)
target_compile_definitions(prio PUBLIC "-DPRIO_VERSION=\"${PROJECT_VERSION_FULL}\"")

if(PRIO_USE_SEXPCPP)
  target_link_libraries(prio PUBLIC sexp::sexp)
  target_compile_definitions(prio PUBLIC PRIO_USE_SEXPCPP)
endif()

if(PRIO_USE_JSONCPP)
  target_link_libraries(prio PUBLIC PkgConfig::JSONCPP)
  target_compile_definitions(prio PUBLIC PRIO_USE_JSONCPP)
endif()

if(BUILD_TESTS)
  find_package(GTest REQUIRED)

  set(TEST_PRIO_SOURCES
    test/float_format_test.cpp
    test/reader_test.cpp
    test/writer_test.cpp
    test/reader_document_test.cpp
    test/reader_mapping_test.cpp)

  if(PRIO_USE_JSONCPP)
    list(APPEND TEST_PRIO_SOURCES
      test/jsonpretty_writer_impl_test.cpp
      test/json_writer_impl_test.cpp)
  endif()

  if(PRIO_USE_SEXPCPP)
    list(APPEND TEST_PRIO_SOURCES
      test/sexpr_writer_impl_test.cpp)
  endif()

  add_executable(test_prio ${TEST_PRIO_SOURCES})
  set_target_properties(test_prio PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF)
  target_compile_options(test_prio PRIVATE ${WARNINGS_CXX_FLAGS})
  target_include_directories(test_prio PRIVATE src/)
  target_link_libraries(test_prio
    prio::prio
    GTest::GTest
    GTest::Main)

  enable_testing()
  add_test(NAME test_prio
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    COMMAND test_prio)
endif()

if(BUILD_EXTRA)
  file(GLOB PRIO_EXTRA_SOURCES_CXX RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    extra/*.cpp)
  foreach(EXTRA_SOURCE ${PRIO_EXTRA_SOURCES_CXX})
    get_filename_component(EXTRA_EXE ${EXTRA_SOURCE} NAME_WE)
    add_executable(${EXTRA_EXE} ${EXTRA_SOURCE})
    set_target_properties(${EXTRA_EXE} PROPERTIES
      CXX_STANDARD 20
      CXX_STANDARD_REQUIRED ON
      CXX_EXTENSIONS OFF
      RUNTIME_OUTPUT_DIRECTORY "extra/")
    target_compile_options(${EXTRA_EXE} PRIVATE ${WARNINGS_CXX_FLAGS})
    target_link_libraries(${EXTRA_EXE} PRIVATE prio::prio)

    if(PRIO_INSTALL)
      install(TARGETS ${EXTRA_EXE}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
    endif()
  endforeach()

  if(BUILD_TESTS)
    # Round-trip smoke test: prefer sexpr input when available, else json.
    if(PRIO_USE_SEXPCPP)
      add_test(NAME priotool
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/extra/priotool --sexp test/data/data.sexp)
    elseif(PRIO_USE_JSONCPP)
      add_test(NAME priotool
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/extra/priotool --json test/data/data.json)
    endif()
  endif()
endif()

if(PRIO_INSTALL)
  # --- install / export (inlined from former tinycmmc_export_and_install_library) ---
  install(TARGETS prio
    EXPORT prio
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/prio)

  configure_package_config_file(
    prio-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/prio-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/prio)

  write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/prio-config-version.cmake
    VERSION "${PROJECT_VERSION}"
    COMPATIBILITY SameMinorVersion)

  export(EXPORT prio
    NAMESPACE prio::
    FILE ${CMAKE_CURRENT_BINARY_DIR}/prio-targets.cmake)

  install(EXPORT prio
    FILE prio-targets.cmake
    NAMESPACE prio::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/prio)

  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/prio-config-version.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/prio-config.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/prio)
endif()

# EOF #
