// prio - Property I/O for C++
// Copyright (C) 2026 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
// License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include <gtest/gtest.h>

#include <bit>
#include <cmath>
#include <cstdint>
#include <locale>
#include <sstream>
#include <string>

#include "float_format.hpp"

using namespace prio;

namespace {

std::string format(float value)
{
  std::ostringstream os;
  write_float(os, value);
  return os.str();
}

/** Writes ',' as the decimal point and groups thousands with '.' */
struct CommaNumpunct : std::numpunct<char>
{
  char do_decimal_point() const override { return ','; }
  char do_thousands_sep() const override { return '.'; }
  std::string do_grouping() const override { return "\3"; }
};

} // namespace

TEST(FloatFormatTest, shortest)
{
  EXPECT_EQ(format(0.0f), "0");
  EXPECT_EQ(format(1.0f), "1");
  EXPECT_EQ(format(-1.5f), "-1.5");
  EXPECT_EQ(format(0.1f), "0.1");
  EXPECT_EQ(format(123.5f), "123.5");
  EXPECT_EQ(format(1234.567f), "1234.567");
  EXPECT_EQ(format(1.234e13f), "1.234e+13");
  // needs more than the 6 digits operator<< writes by default
  EXPECT_EQ(format(1057.3601f), "1057.3601");
  EXPECT_EQ(format(3.2000005f), "3.2000005");
}

TEST(FloatFormatTest, ignores_stream_locale_and_flags)
{
  std::ostringstream os;
  os.imbue(std::locale(std::locale::classic(), new CommaNumpunct));
  os << std::fixed;
  write_float(os, 1234.5f);
  EXPECT_EQ(os.str(), "1234.5");
}

TEST(FloatFormatTest, roundtrip)
{
  // Every 65521st bit pattern, covering all exponents
  for (uint64_t bits = 0; bits <= 0xffffffffu; bits += 65521)
  {
    float const value = std::bit_cast<float>(static_cast<uint32_t>(bits));
    if (!std::isfinite(value)) {
      continue;
    }

    std::string const text = format(value);
    std::istringstream in(text);
    in.imbue(std::locale::classic());
    float result = 0.0f;
    in >> result;
    ASSERT_EQ(std::bit_cast<uint32_t>(value), std::bit_cast<uint32_t>(result))
      << "value " << value << " written as \"" << text << "\"";
  }
}

/* EOF */
