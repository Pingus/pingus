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

cmake_minimum_required(VERSION 3.0)
project(pingus)

include(FindPkgConfig)
include(FindOpenGL)

pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
pkg_search_module(SDL2MIXER REQUIRED SDL2_mixer>=2.0.0)
pkg_search_module(PNG REQUIRED libpng)

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
  src/lisp/*.cpp
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
  src/util/*.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -Wnon-virtual-dtor -Weffc++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wconversion -Wold-style-cast -Werror -Wshadow -Wcast-qual")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Winit-self -Wno-unused-parameter")

include_directories(
  ${SDL2_INCLUDE_DIRS}
  ${SDL2IMAGE_INCLUDE_DIRS})

include_directories(
  src/
  external/tinygettext/include/
  external/logmich/include/)

file(GLOB TINYGETTEXT_SOURCES_CXX RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  external/tinygettext/src/*.cpp)
add_library(tinygettext STATIC ${TINYGETTEXT_SOURCES_CXX})

file(GLOB LOGMICH_SOURCES_CXX RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  external/logmich/src/*.cpp)
add_library(logmich STATIC ${LOGMICH_SOURCES_CXX})

add_library(libpingus STATIC ${PINGUS_SOURCES_CXX})
set_target_properties(libpingus PROPERTIES OUTPUT_NAME pingus)
target_compile_definitions(libpingus PUBLIC -DVERSION="0.8.0")

add_executable(pingus src/main.cpp)
target_link_libraries(pingus
  libpingus
  logmich
  tinygettext
  ${SDL2_LIBRARIES}
  ${SDL2IMAGE_LIBRARIES}
  ${SDL2MIXER_LIBRARIES}
  ${PNG_LIBRARIES}
  ${OPENGL_LIBRARIES})

# # build test and utils
# for filename in Glob("test/*_test.cpp", strings=True):
#     self.env.Alias('test', self.env.Program(filename[:-4], [filename, libpingus]))
# for filename in Glob("test/*_util.cpp", strings=True):
#     self.env.Alias('test', self.env.Program(filename[:-4], [filename, libpingus]))

# # build extra stuff
# for filename in Glob("extra/*.cpp", strings=True):
#     self.env.Alias('extra', self.env.Program(filename[:-4], [filename, libpingus]))

# EOF #
