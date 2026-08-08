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

cmake_minimum_required(VERSION 3.6)
project(geom VERSION 0.0.0)

include(mk/cmake/TinyCMMC.cmake)

function(build_dependencies)
  # Must guard find_package(glm) as otherwise error will result
  if(NOT TARGET glm::glm)
    find_package(glm REQUIRED)
  endif()
endfunction()
build_dependencies()

file(GLOB GEOM_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  include/geom/*.hpp)

add_library(geom INTERFACE)
target_include_directories(geom INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_link_libraries(geom INTERFACE glm::glm)
target_compile_definitions(geom INTERFACE -DGLM_ENABLE_EXPERIMENTAL)
set_target_properties(geom PROPERTIES PUBLIC_HEADER
  "${GEOM_HEADERS}")

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/geom.pc.in"
  "${CMAKE_CURRENT_BINARY_DIR}/geom.pc"
  @ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/geom.pc"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")

if(BUILD_TESTS)
  enable_testing()
  find_package(GTest REQUIRED)

  # build automatic tests
  file(GLOB TEST_GEOM_SOURCES test/*_test.cpp)
  add_executable(test_geom ${TEST_GEOM_SOURCES})
  target_compile_options(test_geom PRIVATE ${TINYCMMC_WARNINGS_CXX_FLAGS})
  target_link_libraries(test_geom
    GTest::GTest
    GTest::Main
    geom)

  set(ARGV "-V")
  add_test(NAME test_geom
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND test_geom)
endif()

tinycmmc_export_and_install_library(geom)

# EOF #
