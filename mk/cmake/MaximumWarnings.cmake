# Copyright (C) 2019 Ingo Ruhnke <grumbel@gmail.com>
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

# Fill the variable ${WARNINGS_CXX_FLAGS} with about as much warnings
# flags as seem sensible for C++. Add WARNINGS and WERROR options to enable the flags

option(WARNINGS "Switch on extra warnings" OFF)

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
      -Wmissing-include-dirs
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
      -Wuseless-cast
      -Wzero-as-null-pointer-constant
      -pedantic
      )
    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 8)
      list(APPEND WARNINGS_CXX_FLAGS -Wint-in-bool-context)
    endif()
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    list(APPEND WARNINGS_CXX_FLAGS
      -Weverything

      # flags that we deliberately ignore
      -Wno-c++98-compat
      -Wno-c++98-compat-pedantic
      -Wno-float-equal
      -Wno-padded
      -Wno-weak-vtables
      -Wno-disabled-macro-expansion
      -Wno-documentation
      -Wno-reserved-id-macro
      -Wno-sign-conversion

      # warnings that should probably be fixed in code
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

option(WERROR "Turn warnings into errors" OFF)

# EOF #
