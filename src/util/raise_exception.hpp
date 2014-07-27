//  Pingus - A free Lemmings clone
//  Copyright (C) 1998-2011 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_PINGUS_UTIL_RAISE_EXCEPTION_HPP
#define HEADER_PINGUS_UTIL_RAISE_EXCEPTION_HPP

#include <sstream>
#include <string>
#include <stdexcept>

#include "util/log.hpp"

inline std::string raise_log_pretty_print(const std::string& str)
{
  // FIXME: very basic, might not work with complex return types
  std::string::size_type function_start = 0;
  for(std::string::size_type i = 0; i < str.size(); ++i)
  {
    if (str[i] == ' ')
    {
      function_start = i+1;
    }
    else if (str[i] == '(')
    {
      return str.substr(function_start, i - function_start) + "()";
    }
  }

  return str.substr(function_start);
}

#define raise_exception(type, expr) do {  \
  std::ostringstream b42465a70169; \
  b42465a70169 << raise_log_pretty_print(__PRETTY_FUNCTION__) << ": " << expr; \
  throw type(b42465a70169.str()); \
} while(false)

#define raise_error(expr) do {  \
  std::ostringstream b42465a70169; \
  b42465a70169 << raise_log_pretty_print(__PRETTY_FUNCTION__) << ": " << expr; \
  throw std::runtime_error(b42465a70169.str()); \
} while(false)

#endif

/* EOF */
