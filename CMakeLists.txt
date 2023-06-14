# Pingus - A free Lemmings clone
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

cmake_minimum_required(VERSION 3.15)
project(pingus VERSION 0.8.0)

include(mk/cmake/TinyCMMC.cmake)

# FIXME: Move this into GetProjectVersion, get date from git
string(TIMESTAMP PROJECT_DATE "%Y-%m-%d" UTC)

find_package(PkgConfig REQUIRED)
set(OpenGL_GL_PREFERENCE "LEGACY")
find_package(OpenGL REQUIRED)
pkg_search_module(SDL2 REQUIRED sdl2 IMPORTED_TARGET)
pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0 IMPORTED_TARGET)
pkg_search_module(PNG REQUIRED libpng IMPORTED_TARGET)
pkg_search_module(SIGCXX REQUIRED sigc++-2.0 IMPORTED_TARGET)
find_package(fmt REQUIRED)
find_package(glm REQUIRED)

if (BUILD_TESTS)
  # add 'make test' target, use 'make test ARGS="-V"' or 'ctest -V' for verbose
  enable_testing()
endif(BUILD_TESTS)

file(GLOB PINGUS_SOURCES_CXX RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  src/editor/*.cpp
  src/engine/display/*.cpp
  src/engine/display/delta/*.cpp
  src/engine/display/opengl/*.cpp
  src/engine/gui/*.cpp
  src/engine/input/*.cpp
  src/engine/resource/*.cpp
  src/engine/screen/*.cpp
  src/engine/sound/*.cpp
  src/engine/system/*.cpp
  src/math/*.cpp
  src/pingus/*.cpp
  src/pingus/actions/*.cpp
  src/pingus/colliders/*.cpp
  src/pingus/components/*.cpp
  src/pingus/movers/*.cpp
  src/pingus/particles/*.cpp
  src/pingus/screens/*.cpp
  src/pingus/worldmap/*.cpp
  src/pingus/worldobjs/*.cpp
  src/util/*.cpp
  )

function(build_dependencies)
  set(BUILD_TESTS OFF)
  set(TINYGETTEXT_WITH_SDL ON)

  tinycmmc_find_dependency(uitest)
  tinycmmc_find_dependency(tinygettext)
  tinycmmc_find_dependency(argpp)
  tinycmmc_find_dependency(geom)
  tinycmmc_find_dependency(logmich)
  tinycmmc_find_dependency(prio)
  tinycmmc_find_dependency(strut)
  tinycmmc_find_dependency(wstsound)
  if(WIN32)
    add_library(xdgcpp INTERFACE)
  else()
    tinycmmc_find_dependency(xdgcpp)
  endif()
endfunction()
build_dependencies()

# Build Pingus
add_library(libpingus STATIC ${PINGUS_SOURCES_CXX})
set_target_properties(libpingus PROPERTIES OUTPUT_NAME pingus)
target_include_directories(libpingus PUBLIC src/)
target_compile_options(libpingus PRIVATE ${TINYCMMC_WARNINGS_CXX_FLAGS})
target_compile_definitions(libpingus PUBLIC
  -DPROJECT_VERSION="${PROJECT_VERSION}"
  -DPROJECT_NAME="${PROJECT_NAME}")
target_link_libraries(libpingus PUBLIC
  argpp::argpp
  geom::geom
  logmich::logmich
  prio::prio
  tinygettext
  strut::strut
  xdgcpp
  wstsound::wstsound
  fmt::fmt
  glm::glm
  PkgConfig::SDL2
  PkgConfig::SDL2IMAGE
  PkgConfig::PNG
  PkgConfig::SIGCXX
  OpenGL::GL)

set(PINGUS_MAIN_SOURCES_CXX src/main.cpp)
if(WIN32)
  list(APPEND PINGUS_MAIN_SOURCES_CXX
    src/win32/console.cpp
    src/win32/pingus.rc)
endif()
add_executable(pingus ${PINGUS_MAIN_SOURCES_CXX})
set_target_properties(libpingus PROPERTIES OUTPUT_NAME pingus)
target_compile_options(pingus PRIVATE ${TINYCMMC_WARNINGS_CXX_FLAGS})
target_link_libraries(pingus libpingus)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data
  DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(BUILD_EXTRA)
  file(GLOB PINGUS_EXTRA_SOURCES_CXX RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    extra/*.cpp)
  foreach(EXTRA_SOURCE ${PINGUS_EXTRA_SOURCES_CXX})
    string(REGEX REPLACE ".*/\([^/]*\).cpp" "\\1" EXTRA_EXE ${EXTRA_SOURCE})
    add_executable(${EXTRA_EXE} ${EXTRA_SOURCE})
    set_target_properties(${EXTRA_EXE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "extra/")
    target_compile_options(${EXTRA_EXE} PRIVATE ${TINYCMMC_WARNINGS_CXX_FLAGS})
    target_link_libraries(${EXTRA_EXE} libpingus)
  endforeach(EXTRA_SOURCE)
endif()

if(BUILD_TESTS)
  find_package(GTest REQUIRED)

  # build interactive tests
  file(GLOB UITEST_PINGUS_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} uitest/*_test.cpp)
  add_executable(uitest_pingus ${UITEST_PINGUS_SOURCES})
  target_link_libraries(uitest_pingus libpingus uitest uitest_main)

  file(GLOB PINGUS_TEST_SOURCES_CXX RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    test/*_test.cpp)
  foreach(TEST_SOURCE ${PINGUS_TEST_SOURCES_CXX})
    string(REGEX REPLACE ".*/\([^/]*\).cpp" "\\1" TEST_EXE ${TEST_SOURCE})
    add_executable(${TEST_EXE} ${TEST_SOURCE})
    set_target_properties(${TEST_EXE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "test/")
    target_compile_options(${TEST_EXE} PRIVATE ${TINYCMMC_WARNINGS_CXX_FLAGS})
    target_link_libraries(${TEST_EXE} libpingus)
  endforeach(TEST_SOURCE)

  file(GLOB PINGUS_UTIL_SOURCES_CXX RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    test/*_util.cpp)
  foreach(UTIL_SOURCE ${PINGUS_UTIL_SOURCES_CXX})
    string(REGEX REPLACE ".*/\([^/]*\).cpp" "\\1" UTIL_EXE ${UTIL_SOURCE})
    add_executable(${UTIL_EXE} ${UTIL_SOURCE})
    set_target_properties(${UTIL_EXE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "test/")
    target_compile_options(${UTIL_EXE} PRIVATE ${TINYCMMC_WARNINGS_CXX_FLAGS})
    target_link_libraries(${UTIL_EXE} libpingus)
  endforeach(UTIL_SOURCE)

  file(GLOB TEST_PINGUS_SOURCES_CXX RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    tests/*_test.cpp)
  add_executable(test_pingus ${TEST_PINGUS_SOURCES_CXX})
  target_include_directories(test_pingus PUBLIC src/ tests/)
  target_link_libraries(test_pingus libpingus GTest::GTest GTest::Main ${CMAKE_THREAD_LIBS_INIT})
  target_compile_options(test_pingus PRIVATE ${TINYCMMC_WARNINGS_CXX_FLAGS})

  add_test(NAME test_pingus
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND test_pingus)

  add_test(NAME test_pingus_desktop
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND desktop-file-validate pingus.desktop)
endif(BUILD_TESTS)

if(WIN32)
  install(TARGETS pingus
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
else()
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/pingus.sh.in
    ${CMAKE_BINARY_DIR}/pingus.sh)

  install(TARGETS pingus
    RUNTIME DESTINATION ${CMAKE_INSTALL_LIBEXECDIR})

  install(FILES
    ${CMAKE_BINARY_DIR}/pingus.sh
    RENAME pingus
    PERMISSIONS OWNER_EXECUTE OWNER_READ OWNER_WRITE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
    DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

install(DIRECTORY data/ DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}
  PATTERN "*~" EXCLUDE)

install(FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/doc/man/pingus.6
  DESTINATION ${CMAKE_INSTALL_MANDIR}/man6)

install(FILES
  pingus.desktop
  DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)

install(DIRECTORY data/icons/
  DESTINATION ${CMAKE_INSTALL_DATADIR}/icons)

configure_file(pingus.appdata.xml.in ${CMAKE_CURRENT_BINARY_DIR}/pingus.appdata.xml)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/pingus.appdata.xml
  DESTINATION ${CMAKE_INSTALL_DATADIR}/metainfo)

if (BUILD_TESTS)
  add_test(NAME pingus.appdata.xml
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND appstream-util validate-relax ${CMAKE_CURRENT_BINARY_DIR}/pingus.appdata.xml)
endif(BUILD_TESTS)

# EOF #
