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
project(pingus)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(mk/cmake/TinyCMMC.cmake)

# Port / packaging options (used by mk/wasm, mk/r36s, distro builds).
set(SDL2_ROOT "" CACHE PATH "Prebuilt SDL2 prefix (wasm static libs, optional)")
set(DATA_PREFIX "" CACHE PATH "Runtime data directory (empty = compile-time default)")
if(NOT PROJECT_VERSION_FULL)
  set(PROJECT_VERSION_FULL "${PROJECT_VERSION}" CACHE STRING "Full version string" FORCE)
endif()

option(PINGUS_USE_GLES "Prefer OpenGL ES (Emscripten / some embedded)" OFF)
if(EMSCRIPTEN)
  set(PINGUS_USE_GLES ON)
  message(STATUS "Emscripten: enabling PINGUS_USE_GLES")
endif()

# Optional prebuilt SDL prefix (wasm): pkg-config + find_path under SDL2_ROOT.
if(SDL2_ROOT)
  list(APPEND CMAKE_PREFIX_PATH "${SDL2_ROOT}")
  set(ENV{PKG_CONFIG_PATH} "${SDL2_ROOT}/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}")
  message(STATUS "SDL2_ROOT=${SDL2_ROOT}")
endif()

find_package(PkgConfig REQUIRED)

if(PINGUS_USE_GLES OR EMSCRIPTEN)
  # GLES / WebGL path — still uses the existing fixed-function-ish client in
  # places; full ES2 rewrite is future work. Find GLES2 when available.
  find_library(PINGUS_GLESV2_LIB GLESv2)
  find_library(PINGUS_EGL_LIB EGL)
  if(EMSCRIPTEN)
    # Emscripten provides GL via link flags (-sUSE_WEBGL2 / FULL_ES2).
    set(PINGUS_GL_LIBS "")
  elseif(PINGUS_GLESV2_LIB)
    set(PINGUS_GL_LIBS ${PINGUS_GLESV2_LIB})
    if(PINGUS_EGL_LIB)
      list(APPEND PINGUS_GL_LIBS ${PINGUS_EGL_LIB})
    endif()
  else()
    message(WARNING "PINGUS_USE_GLES set but GLESv2 not found; falling back to OpenGL")
    set(OpenGL_GL_PREFERENCE "LEGACY")
    find_package(OpenGL REQUIRED)
    set(PINGUS_GL_LIBS OpenGL::GL)
  endif()
else()
  set(OpenGL_GL_PREFERENCE "LEGACY")
  find_package(OpenGL REQUIRED)
  set(PINGUS_GL_LIBS OpenGL::GL)
endif()

if(EMSCRIPTEN AND SDL2_ROOT)
  # Emscripten's toolchain sets FIND_ROOT_PATH_MODE_*=ONLY, so find_path /
  # find_library ignore SDL2_ROOT outside the emscripten sysroot. Resolve paths
  # explicitly instead.
  set(_pingus_sdl2_h "")
  if(EXISTS "${SDL2_ROOT}/include/SDL2/SDL.h")
    set(_pingus_sdl2_h "${SDL2_ROOT}/include/SDL2/SDL.h")
    set(SDL2_INCLUDE_DIR "${SDL2_ROOT}/include")
  elseif(EXISTS "${SDL2_ROOT}/include/SDL.h")
    set(_pingus_sdl2_h "${SDL2_ROOT}/include/SDL.h")
    set(SDL2_INCLUDE_DIR "${SDL2_ROOT}/include")
  endif()

  set(SDL2_LIBRARY "")
  foreach(_cand libSDL2.a libSDL2-static.a)
    if(EXISTS "${SDL2_ROOT}/lib/${_cand}")
      set(SDL2_LIBRARY "${SDL2_ROOT}/lib/${_cand}")
      break()
    endif()
  endforeach()

  set(SDL2_IMAGE_LIBRARY "")
  foreach(_cand libSDL2_image.a)
    if(EXISTS "${SDL2_ROOT}/lib/${_cand}")
      set(SDL2_IMAGE_LIBRARY "${SDL2_ROOT}/lib/${_cand}")
      break()
    endif()
  endforeach()

  if(NOT _pingus_sdl2_h OR NOT SDL2_LIBRARY)
    message(FATAL_ERROR
      "EMSCRIPTEN build: SDL2 not found under SDL2_ROOT=${SDL2_ROOT}\n"
      "  expected include/SDL2/SDL.h (or include/SDL.h) and lib/libSDL2.a")
  endif()

  message(STATUS "Emscripten SDL2 include: ${SDL2_INCLUDE_DIR}")
  message(STATUS "Emscripten SDL2 library: ${SDL2_LIBRARY}")
  if(SDL2_IMAGE_LIBRARY)
    message(STATUS "Emscripten SDL2_image: ${SDL2_IMAGE_LIBRARY}")
  endif()

  add_library(pingus_sdl2 INTERFACE)
  target_include_directories(pingus_sdl2 INTERFACE
    "${SDL2_INCLUDE_DIR}"
    "${SDL2_ROOT}/include")
  target_link_libraries(pingus_sdl2 INTERFACE "${SDL2_LIBRARY}")
  if(SDL2_IMAGE_LIBRARY)
    target_link_libraries(pingus_sdl2 INTERFACE "${SDL2_IMAGE_LIBRARY}")
  endif()
  set(PINGUS_SDL_LIBS pingus_sdl2)
else()
  pkg_search_module(SDL2 REQUIRED sdl2 IMPORTED_TARGET)
  pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0 IMPORTED_TARGET)
  set(PINGUS_SDL_LIBS PkgConfig::SDL2 PkgConfig::SDL2IMAGE)
endif()

# Optional / platform-variable deps
set(PINGUS_OPTIONAL_LIBS "")
if(NOT EMSCRIPTEN)
  pkg_search_module(SIGCXX REQUIRED sigc++-2.0 IMPORTED_TARGET)
  list(APPEND PINGUS_OPTIONAL_LIBS PkgConfig::SIGCXX)
else()
  message(STATUS "Emscripten: sigc++ lookup optional")
  pkg_search_module(SIGCXX QUIET sigc++-2.0 IMPORTED_TARGET)
  if(SIGCXX_FOUND)
    list(APPEND PINGUS_OPTIONAL_LIBS PkgConfig::SIGCXX)
  else()
    message(WARNING "sigc++-2.0 not found — Emscripten build needs it in the sysroot/ports")
  endif()
endif()

find_package(glm REQUIRED)

if(WIN32)
  # Fix for this issue:
  # include/mcfgthread/fwd.h:24:4: error:
  # #error Please define _WIN32_WINNT to at least Windows 7.
  add_definitions(-D_WIN32_WINNT=0x0601)
endif()

option(PINGUS_ENABLE_SOUND "Build wstsound-backed audio (OFF for wasm/minimal)" ON)
if(EMSCRIPTEN)
  set(PINGUS_ENABLE_SOUND OFF CACHE BOOL "Build wstsound-backed audio" FORCE)
  add_definitions(-DPINGUS_EMSCRIPTEN=1)
  if(PINGUS_USE_GLES)
    add_definitions(-DPINGUS_USE_GLES=1)
  endif()
endif()
if(NOT PINGUS_ENABLE_SOUND)
  add_definitions(-DPINGUS_NO_SOUND=1)
  message(STATUS "PINGUS_ENABLE_SOUND=OFF — dummy audio only")
endif()
if(PINGUS_USE_GLES)
  add_definitions(-DPINGUS_USE_GLES=1)
else()
  # Desktop GL 3.3 path: expose shader/VBO prototypes from system glext headers.
  add_definitions(-DGL_GLEXT_PROTOTYPES=1)
endif()

if(BUILD_TESTS)
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

if(NOT PINGUS_ENABLE_SOUND)
  list(FILTER PINGUS_SOURCES_CXX EXCLUDE REGEX ".*/sound_real\.cpp$")
endif()

function(build_dependencies)
  set(BUILD_TESTS OFF)
  set(TINYGETTEXT_WITH_SDL ON)

  # Order matters when using external/ via add_subdirectory: sexp + logmich
  # before prio (priocpp links them and checks TARGET …::…).
  tinycmmc_find_dependency(uitest)
  tinycmmc_find_dependency(tinygettext)
  tinycmmc_find_dependency(argpp)
  tinycmmc_find_dependency(geom)
  tinycmmc_find_dependency(logmich)
  tinycmmc_find_dependency(sexp)
  if(EMSCRIPTEN)
    # jsoncpp is not in the emscripten sysroot; game data uses sexp/prio paths.
    set(PRIO_USE_JSONCPP OFF CACHE BOOL "" FORCE)
  endif()
  tinycmmc_find_dependency(prio)
  tinycmmc_find_dependency(strut)
  if(PINGUS_ENABLE_SOUND)
    tinycmmc_find_dependency(wstsound)
  else()
    add_library(wstsound INTERFACE)
    add_library(wstsound::wstsound ALIAS wstsound)
  endif()
  if(WIN32 OR EMSCRIPTEN OR ANDROID)
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
  -DPROJECT_VERSION="${PROJECT_VERSION_FULL}"
  -DPROJECT_NAME="${PROJECT_NAME}")
if(DATA_PREFIX)
  target_compile_definitions(libpingus PUBLIC
    -DPINGUS_DEFAULT_DATADIR="${DATA_PREFIX}")
  message(STATUS "PINGUS_DEFAULT_DATADIR=${DATA_PREFIX}")
endif()
target_link_libraries(libpingus PUBLIC
  argpp::argpp
  geom::geom
  logmich::logmich
  prio::prio
  tinygettext
  strut::strut
  xdgcpp
  glm::glm
  ${PINGUS_SDL_LIBS}
  ${PINGUS_OPTIONAL_LIBS}
  ${PINGUS_GL_LIBS})
if(PINGUS_ENABLE_SOUND)
  target_link_libraries(libpingus PUBLIC wstsound::wstsound)
endif()

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

if(EMSCRIPTEN)
  # Produce pingus.html / .js / .wasm; link flags come from the wasm build script.
  set_target_properties(pingus PROPERTIES SUFFIX ".html")
  if(DEFINED EMSCRIPTEN_LINK_FLAGS)
    # cmake -DEMSCRIPTEN_LINK_FLAGS="..." may be a single string of flags.
    separate_arguments(_pingus_em_link NATIVE_COMMAND "${EMSCRIPTEN_LINK_FLAGS}")
    target_link_options(pingus PRIVATE ${_pingus_em_link})
  endif()
endif()

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

# EOF #
