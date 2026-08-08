# UITest - Simple testing framework for interactive tests
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
project(uitest VERSION 0.0.0)

include(mk/cmake/TinyCMMC.cmake)

# build uitest
add_library(uitest STATIC src/arg_info.cpp src/uitest.cpp)
set_target_properties(uitest PROPERTIES PUBLIC_HEADER
  "include/uitest/arg_info.hpp;include/uitest/uitest.hpp")
set_property(TARGET uitest
  PROPERTY CXX_STANDARD 17
  PROPERTY CXX_STANDARD_REQUIRED ON
  PROPERTY CXX_EXTENSIONS OFF)
target_compile_options(uitest PRIVATE ${WARNINGS_CXX_FLAGS})
target_include_directories(uitest PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/uitest>)

add_library(uitest_main STATIC src/uitest_main.cpp)
set_property(TARGET uitest_main
    PROPERTY CXX_STANDARD 17
    PROPERTY CXX_STANDARD_REQUIRED ON
    PROPERTY CXX_EXTENSIONS OFF)
target_compile_options(uitest_main PRIVATE ${WARNINGS_CXX_FLAGS})
target_include_directories(uitest_main SYSTEM PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/uitest>)

# build examples
if(BUILD_EXAMPLES)
  add_executable(example_test examples/examples.cpp)
  target_compile_options(example_test PRIVATE ${WARNINGS_CXX_FLAGS})
  target_link_libraries(example_test LINK_PUBLIC uitest uitest_main)
  target_include_directories(example_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
endif()

if(BUILD_TESTS)
  find_package(GTest REQUIRED)

  # build tests
  add_executable(test_uitest test/arg_info_test.cpp)
  set_property(TARGET test_uitest
    PROPERTY CXX_STANDARD 17
    PROPERTY CXX_STANDARD_REQUIRED ON
    PROPERTY CXX_EXTENSIONS OFF)
  target_compile_options(test_uitest PRIVATE ${WARNINGS_CXX_FLAGS})
  target_include_directories(test_uitest PUBLIC include/)
  target_link_libraries(test_uitest PUBLIC uitest GTest::GTest GTest::Main)

  # add 'make test' target, use 'make test ARGS="-V"' or 'ctest -V' for verbose
  enable_testing()
  add_test(NAME test_uitest
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND test_uitest)
endif()

# # Installation
# install(TARGETS uitest_main uitest
#   EXPORT uitest-targets
#   ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#   PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/uitest)

# # CMake integration
# add_library(uitest::uitest ALIAS uitest)
# add_library(uitest:uitest_main ALIAS uitest_main)

# configure_file(uitest-config.cmake.in
#   "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/uitest-config.cmake" @ONLY)
# install(FILES
#   "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/uitest-config.cmake"
#   DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/uitest/")
# export(TARGETS uitest uitest_main
#   FILE "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/uitest-targets.cmake")
# export(PACKAGE uitest)
# install(EXPORT uitest-targets
#   DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/uitest/")

install(TARGETS uitest uitest_main
  EXPORT uitest
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}")

add_library(uitest::uitest ALIAS uitest)
add_library(uitest:uitest_main ALIAS uitest_main)

include(CMakePackageConfigHelpers)
configure_package_config_file("${PROJECT_NAME}-config.cmake.in" "${PROJECT_NAME}-config.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}")
write_basic_package_version_file("${PROJECT_NAME}-config-version.cmake"
  VERSION "${PROJECT_VERSION}"
  COMPATIBILITY SameMinorVersion)
export(EXPORT "${PROJECT_NAME}"
  NAMESPACE "${PROJECT_NAME}::"
  FILE "${PROJECT_NAME}-targets.cmake")
install(EXPORT "${PROJECT_NAME}"
  FILE "${PROJECT_NAME}-targets.cmake"
  NAMESPACE "${PROJECT_NAME}::"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

# EOF #

