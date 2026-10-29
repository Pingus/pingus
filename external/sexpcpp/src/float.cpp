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

#include <cmath>
#include <limits>
#include <locale>
#include <sstream>

#if defined(SEXP_HAVE_FROM_CHARS_FLOAT) || defined(SEXP_HAVE_TO_CHARS_FLOAT)
#  include <cassert>
#  include <charconv>
#endif

namespace sexp {

namespace detail {

namespace {

bool is_digit(char c)
{
  return '0' <= c && c <= '9';
}

/** Length of the longest prefix of 'text' that is a decimal floating
    point number: [+-]digits[.digits][(e|E)[+-]digits], with digits on at
    least one side of the '.' */
size_t float_prefix_length(std::string_view text)
{
  size_t i = 0;
  if (i < text.size() && (text[i] == '+' || text[i] == '-')) {
    i += 1;
  }

  size_t digits = 0;
  for (; i < text.size() && is_digit(text[i]); ++i) {
    digits += 1;
  }

  if (i < text.size() && text[i] == '.') {
    i += 1;
    for (; i < text.size() && is_digit(text[i]); ++i) {
      digits += 1;
    }
  }

  if (digits == 0) {
    return 0;
  }

  // The exponent only belongs to the number if it has digits
  if (i < text.size() && (text[i] == 'e' || text[i] == 'E')) {
    size_t j = i + 1;
    if (j < text.size() && (text[j] == '+' || text[j] == '-')) {
      j += 1;
    }
    size_t const exponent_start = j;
    for (; j < text.size() && is_digit(text[j]); ++j) {}
    if (j > exponent_start) {
      i = j;
    }
  }

  return i;
}

} // namespace

float string2float_fallback(std::string_view text)
{
  // The classic locale makes this independent of setlocale() and
  // std::locale::global(), unlike strtof(). The standard library hands
  // the actual conversion to the C library (strtof_l() in libstdc++ and
  // libc++), which rounds correctly.
  std::istringstream in(std::string(text.substr(0, float_prefix_length(text))));
  in.imbue(std::locale::classic());
  float result = 0.0f;
  in >> result;

  // iostreams give the largest float and failbit on overflow, strtof()
  // and std::from_chars() give infinity
  if (in.fail() && std::abs(result) == std::numeric_limits<float>::max()) {
    return std::copysign(std::numeric_limits<float>::infinity(), result);
  }
  return result;
}

void float2string_fallback(std::ostream& os, float value)
{
  std::ostringstream out;
  out.imbue(std::locale::classic());

  if (!std::isfinite(value)) {
    out << value;
    os << out.str();
    return;
  }

  // Use the fewest digits that read back to the same value, max_digits10
  // always does
  for (int precision = std::numeric_limits<float>::digits10;
       precision <= std::numeric_limits<float>::max_digits10;
       ++precision)
  {
    out.str(std::string());
    out.precision(precision);
    out << value;
    if (string2float_fallback(out.str()) == value) {
      break;
    }
  }
  os << out.str();
}

} // namespace detail

float string2float(const std::string& text)
{
#ifdef SEXP_HAVE_FROM_CHARS_FLOAT
  char const* start = text.data();
  char const* const end = text.data() + text.size();

  // A leading + (e.g. "+5") is not accepted by from_chars(), so skip it
  if (start != end && *start == '+') {
    start += 1;
  }

  float result = 0.0f;
  auto const [ptr, ec] = std::from_chars(start, end, result);
  if (ec == std::errc::result_out_of_range) {
    // from_chars() leaves 'result' untouched when the value is too large
    // or too small for a float, go through double to get inf or 0
    double value = 0.0;
    std::from_chars(start, end, value);
    return static_cast<float>(value);
  }
  assert(ec == std::errc());
  return result;
#else
  return detail::string2float_fallback(text);
#endif
}

void float2string(std::ostream& os, float value)
{
#ifdef SEXP_HAVE_TO_CHARS_FLOAT
  // 32 is plenty: sign, 9 digits, '.', 'e', exponent sign and 2 digits
  constexpr size_t len = 32;
  char buffer[len];
  auto const result = std::to_chars(buffer, buffer + len, value);
  assert(result.ec == std::errc());
  os.write(buffer, result.ptr - buffer);
#else
  detail::float2string_fallback(os, value);
#endif
}

} // namespace sexp

/* EOF */
