// SExp - A S-Expression Parser for C++
// Copyright (C) 2018 Ingo Ruhnke <grumbel@gmail.com>
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

#include <gtest/gtest.h>

#include <bit>
#include <cmath>
#include <cstdint>
#include <limits>
#include <locale>
#include <string>
#include <sstream>
#include <vector>

#include "float.hpp"

namespace {

using String2Float = float (*)(std::string_view);
using Float2String = void (*)(std::ostream&, float);

float string2float_public(std::string_view text)
{
  return sexp::string2float(std::string(text));
}

/** Run every test against the public functions, which use
    std::from_chars()/std::to_chars() when available, and against the
    fallback implementations */
struct FloatImpl
{
  char const* name;
  String2Float string2float;
  Float2String float2string;
};

class FloatTest : public ::testing::TestWithParam<FloatImpl> {};

std::string to_string(Float2String float2string, float value)
{
  std::ostringstream os;
  float2string(os, value);
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

TEST_P(FloatTest, float2string)
{
  std::vector<std::tuple<float, std::string> > datas =
    {
      { 1.0F, "1" },
      { 1234.567F, "1234.567" },
      { 1.0e3F, "1000" },
      { 1234567e-4F, "123.4567" },
      { 1.234e13F, "1.234e+13" },
      { 1234.0e13F, "1.234e+16" },
      { -1.0F, "-1" },
      { -1234.567F, "-1234.567" },
      { -1.0e3F, "-1000" },
      { -1234567e-4F, "-123.4567" },
      { -1.234e13F, "-1.234e+13" },
      { -1234.0e13F, "-1.234e+16" },
      { 0.1F, "0.1" },

      // FIXME: std::ostream and std::to_chars() have different ideas
      // here, "1234.123" vs "1234.1234", using 1234.1230 as workaround
      { static_cast<float>(1234.123056789012345678901234567890123456789), "1234.123" },
      { static_cast<float>(-1234.123056789012345678901234567890123456789), "-1234.123" },
    };

  for(const auto& data: datas)
  {
    ASSERT_EQ(to_string(GetParam().float2string, std::get<0>(data)), std::get<1>(data));
  }
}

TEST_P(FloatTest, float2string_ignores_stream_locale)
{
  std::ostringstream os;
  os.imbue(std::locale(std::locale::classic(), new CommaNumpunct));
  os << std::fixed;
  GetParam().float2string(os, 1234.5F);
  ASSERT_EQ(os.str(), "1234.5");
}

TEST_P(FloatTest, string2float)
{
  String2Float const string2float = GetParam().string2float;

  ASSERT_EQ(1234.5678F, string2float("1234.5678"));
  ASSERT_EQ(1234.5678F, string2float("+1234.5678"));
  ASSERT_EQ(-1234.5678F, string2float("-1234.5678"));
  ASSERT_EQ(0.5F, string2float(".5"));
  ASSERT_EQ(5.0F, string2float("5."));
  ASSERT_EQ(0.1F, string2float("0.1"));

  ASSERT_EQ(5.0F, string2float("5.0ef"));
  ASSERT_EQ(1234.5F, string2float("1.2345e3f"));
  ASSERT_EQ(12.345F, string2float("12345e-3f"));
}

TEST_P(FloatTest, string2float_out_of_range)
{
  String2Float const string2float = GetParam().string2float;

  ASSERT_EQ(std::numeric_limits<float>::infinity(), string2float("1e40"));
  ASSERT_EQ(-std::numeric_limits<float>::infinity(), string2float("-1e40"));
  ASSERT_EQ(0.0F, string2float("1e-50"));
}

TEST_P(FloatTest, roundtrip)
{
  // Every 65521st bit pattern, covering all exponents
  for (uint64_t bits = 0; bits <= 0xffffffffu; bits += 65521)
  {
    float const value = std::bit_cast<float>(static_cast<uint32_t>(bits));
    if (!std::isfinite(value)) {
      continue;
    }

    std::string const text = to_string(GetParam().float2string, value);
    float const result = GetParam().string2float(text);
    ASSERT_EQ(std::bit_cast<uint32_t>(value), std::bit_cast<uint32_t>(result))
      << "value " << value << " written as \"" << text << "\"";
  }
}

INSTANTIATE_TEST_SUITE_P(
  FloatImpls, FloatTest,
  ::testing::Values(FloatImpl{"public", &string2float_public, &sexp::float2string},
                    FloatImpl{"fallback", &sexp::detail::string2float_fallback, &sexp::detail::float2string_fallback}),
  [](::testing::TestParamInfo<FloatImpl> const& param_info) { return std::string(param_info.param.name); });

/* EOF */
