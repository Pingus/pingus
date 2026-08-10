# geom - Basic collection of point, size and rect classes
# Copyright (C) 2020 Ingo Ruhnke <grumbel@gmail.com>
#
# This software is provided 'as-is', without any express or implied
# warranty.  In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented; you must not
#    claim that you wrote the original software. If you use this software
#    in a product, an acknowledgment in the product documentation would be
#    appreciated but is not required.
# 2. Altered source versions must be plainly marked as such, and must not be
#    misrepresented as being the original software.
# 3. This notice may not be removed or altered from any source distribution.

cmake_minimum_required(VERSION 3.15)
project(geom VERSION 0.0.0)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

option(BUILD_TESTS "Build test cases" OFF)
option(WARNINGS "Enable extra compiler warnings" OFF)
option(WERROR "Treat warnings as errors" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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

if(NOT TARGET glm::glm)
  find_package(glm REQUIRED)
endif()

file(GLOB GEOM_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  include/geom/*.hpp)

add_library(geom INTERFACE)
add_library(geom::geom ALIAS geom)
target_include_directories(geom INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_link_libraries(geom INTERFACE glm::glm)
target_compile_definitions(geom INTERFACE -DGLM_ENABLE_EXPERIMENTAL)
set_target_properties(geom PROPERTIES PUBLIC_HEADER "${GEOM_HEADERS}")

if(BUILD_TESTS)
  enable_testing()
  find_package(GTest REQUIRED)

  file(GLOB TEST_GEOM_SOURCES test/*_test.cpp)
  add_executable(test_geom ${TEST_GEOM_SOURCES})
  target_compile_options(test_geom PRIVATE ${WARNINGS_CXX_FLAGS})
  target_link_libraries(test_geom
    GTest::GTest
    GTest::Main
    geom::geom)
  add_test(NAME test_geom
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND test_geom)
endif()

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/geom.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/geom.pc"
    @ONLY)
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/geom.pc"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")

  install(TARGETS geom
    EXPORT geom
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/geom)

  configure_package_config_file(
    geom-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/geom-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/geom)

  write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/geom-config-version.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMinorVersion)

  export(EXPORT geom
    NAMESPACE geom::
    FILE ${CMAKE_CURRENT_BINARY_DIR}/geom-targets.cmake)

  install(EXPORT geom
    FILE geom-targets.cmake
    NAMESPACE geom::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/geom)

  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/geom-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/geom-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/geom)
endif()

# EOF #
