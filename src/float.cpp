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

#include "float.hpp"

#include <cassert>
#include <charconv>
#include <limits>
#include <sstream>

namespace sexp {

float string2float(const std::string& text)
{
  char const* start = text.data();

  // A leading + (e.g. "+5") is not accepted by from_chars(), so skip it
  if (!text.empty() && text[0] == '+') {
    start += 1;
  }

  float result;
  [[maybe_unused]] auto err = std::from_chars(start, text.data() + text.size(), result);
  assert(err.ec == std::errc());
  return result;
}

void float2string(std::ostream& os, float value)
{
  constexpr size_t len = 32;
  char buffer[len];
  auto result = std::to_chars(buffer, buffer + len, value);
  assert(result.ec == std::errc());
  os.write(buffer, result.ptr - buffer);
}

} // namespace sexp

/* EOF */
