// SExp - A S-Expression Parser for C++
// Copyright (C) 2006 Matthias Braun <matze@braunis.de>
//               2018 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_STRING_HPP
#define HEADER_STRING_HPP

#include <ostream>
#include <string>
#include <string_view>

namespace sexp {

/** Parse the longest prefix of 'text' that is a decimal floating point
    number, independent of the C and C++ locale. Uses std::from_chars()
    when the standard library provides it for float, see CMakeLists.txt. */
float string2float(const std::string& text);

/** Write 'value' in the shortest form that string2float() reads back to
    the same value, independent of the locale and flags of 'os' */
void float2string(std::ostream& os, float value);

namespace detail {

/** Implementations for standard libraries without std::from_chars() and
    std::to_chars() for float, exposed for testing */
float string2float_fallback(std::string_view text);
void float2string_fallback(std::ostream& os, float value);

} // namespace detail

} // namespace sexp

#endif

/* EOF */
