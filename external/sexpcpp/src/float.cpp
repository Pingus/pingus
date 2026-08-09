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
#include <cstdio>
#include <cstdlib>
#include <limits>
#include <sstream>

// Floating-point std::from_chars / std::to_chars are missing on Android
// NDK libc++ and on older libstdc++ (ArkOS / R36S sysroot, GCC ~9 era).
#if !defined(__ANDROID__) && !defined(SEXP_NO_FLOAT_CHARCONV)
#  include <charconv>
#endif

namespace sexp {

float string2float(const std::string& text)
{
  char const* start = text.data();

  // A leading + (e.g. "+5") is not accepted by from_chars(), so skip it
  if (!text.empty() && text[0] == '+') {
    start += 1;
  }

  float result;
#if defined(__ANDROID__) || defined(SEXP_NO_FLOAT_CHARCONV)
  char* end = nullptr;
  result = std::strtof(start, &end);
  assert(end != start);
#else
  [[maybe_unused]] auto err = std::from_chars(start, text.data() + text.size(), result);
  assert(err.ec == std::errc());
#endif
  return result;
}

void float2string(std::ostream& os, float value)
{
  constexpr size_t len = 32;
  char buffer[len];
#if defined(__ANDROID__) || defined(SEXP_NO_FLOAT_CHARCONV)
  int n = std::snprintf(buffer, len, "%g", static_cast<double>(value));
  assert(n > 0 && static_cast<size_t>(n) < len);
  os.write(buffer, n);
#else
  auto result = std::to_chars(buffer, buffer + len, value);
  assert(result.ec == std::errc());
  os.write(buffer, result.ptr - buffer);
#endif
}

} // namespace sexp

/* EOF */
