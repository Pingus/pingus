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

cmake_minimum_required(VERSION 3.10)
project(prio)

include(mk/cmake/TinyCMMC.cmake)

option(PRIO_USE_JSONCPP "Enable support for jsoncpp" ON)
option(PRIO_USE_SEXPCPP "Enable support for sexp-cpp" ON)

find_package(fmt REQUIRED)
find_package(PkgConfig REQUIRED)

if(PRIO_USE_JSONCPP)
  pkg_search_module(JSONCPP REQUIRED jsoncpp IMPORTED_TARGET)
endif()

function(build_dependencies)
  set(BUILD_TESTS OFF)

  if(PRIO_USE_SEXPCPP)
    find_package(sexp QUIET)
    if(NOT TARGET sexp::sexp)
      add_subdirectory(external/sexp-cpp/ EXCLUDE_FROM_ALL)
    endif()
  endif()

find_package(logmich QUIET)
  if(NOT TARGET logmich::logmich)
    add_subdirectory(external/logmich/ EXCLUDE_FROM_ALL)
  endif()
endfunction()
build_dependencies()

set(PRIO_HEADERS
  include/prio/error_handler.hpp
  include/prio/format.hpp
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
set_target_properties(prio PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF)
target_compile_options(prio PRIVATE ${WARNINGS_CXX_FLAGS})
target_include_directories(prio INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_include_directories(prio PRIVATE src/ include/prio/)
set_target_properties(prio PROPERTIES PUBLIC_HEADER "${PRIO_HEADERS}")
target_link_libraries(prio PUBLIC
  logmich::logmich
  fmt::fmt)

if(PRIO_USE_SEXPCPP)
  target_link_libraries(prio PUBLIC sexp::sexp)
  target_compile_definitions(prio PUBLIC "-DPRIO_USE_SEXPCPP=1")
endif()

if(PRIO_USE_JSONCPP)
  target_link_libraries(prio PUBLIC PkgConfig::JSONCPP)
  target_compile_definitions(prio PUBLIC "-DPRIO_USE_JSONCPP=1")
endif()

if(BUILD_TESTS)
  if(NOT TARGET GTest)
    find_package(GTest REQUIRED)
  endif()

  set(TEST_PRIO_SOURCES
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
    string(REGEX REPLACE ".*/\([^/]*\).cpp" "\\1" EXTRA_EXE ${EXTRA_SOURCE})
    add_executable(${EXTRA_EXE} ${EXTRA_SOURCE})
    set_target_properties(${EXTRA_EXE} PROPERTIES
      CXX_STANDARD 20
      CXX_STANDARD_REQUIRED ON
      CXX_EXTENSIONS OFF)
    set_target_properties(${EXTRA_EXE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "extra/")
    target_compile_options(${EXTRA_EXE} PRIVATE ${WARNINGS_CXX_FLAGS})
    target_link_libraries(${EXTRA_EXE} prio)

    install(TARGETS ${EXTRA_EXE}
      RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
  endforeach(EXTRA_SOURCE)

  if(BUILD_TESTS)
    add_test(NAME priotool
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
      COMMAND ${CMAKE_CURRENT_BINARY_DIR}/extra/priotool test/data/data.sexp)
  endif()
endif()

tinycmmc_export_and_install_library(prio)

# EOF #
