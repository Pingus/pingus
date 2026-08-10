# Copyright Ingo Ruhnke <grumbel@gmail.com>
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

# Requires the following files from
# https://github.com/rpavlik/cmake-modules:
# * GetGitRevisionDescription.cmake
# * GetGitRevisionDescription.cmake.in

include(GetGitRevisionDescription)

# Prefer the current project's source tree so version detection works when
# the library is consumed via add_subdirectory() from a parent project.
function(get_project_version _outputvar)
  set(_root "${CMAKE_CURRENT_SOURCE_DIR}")

  if(EXISTS "${_root}/.git")
    git_describe(GIT_REPO_VERSION "--tags" "--match" "v[0-9]*.[0-9]*.[0-9]*")
    string(REGEX REPLACE "^v([0-9].*)" "\\1" CLEANED_GIT_REPO_VERSION "${GIT_REPO_VERSION}")

    if(CLEANED_GIT_REPO_VERSION)
      set(${_outputvar} "${CLEANED_GIT_REPO_VERSION}" PARENT_SCOPE)
    else()
      set(${_outputvar} "${GIT_REPO_VERSION}" PARENT_SCOPE)
    endif()
  elseif(EXISTS "${_root}/VERSION")
    file(STRINGS "${_root}/VERSION" _ver)

    if(_ver MATCHES "^\\$")
      # gitattribute $Format$ was not expanded
      set(${_outputvar} "unknown-version" PARENT_SCOPE)
    else()
      # strip leading 'v', in case VERSION is generated from "git describe"
      string(REGEX REPLACE "^v(.*)" "\\1" _ver "${_ver}")
      set(${_outputvar} "${_ver}" PARENT_SCOPE)
    endif()
  else()
    # obtain version from directory name
    get_filename_component(BASENAME "${_root}" NAME)
    string(REGEX REPLACE "^${PROJECT_NAME}[-_]v?(.*)" "\\1" DIRECTORY_VERSION "${BASENAME}")
    if(NOT "${DIRECTORY_VERSION}" STREQUAL "${BASENAME}")
      set(${_outputvar} "${DIRECTORY_VERSION}" PARENT_SCOPE)
    else()
      set(${_outputvar} "unknown-version" PARENT_SCOPE)
    endif()
  endif()
endfunction()

get_project_version(PROJECT_VERSION)

message(STATUS "Project Name: ${PROJECT_NAME}")
message(STATUS "Project Version: ${PROJECT_VERSION}")

# EOF #
