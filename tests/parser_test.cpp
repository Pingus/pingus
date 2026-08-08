// SExp - A S-Expression Parser for C++
// Copyright (C) 2015 Ingo Ruhnke <grumbel@gmail.com>
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

#include <iostream>
#include <sstream>

#include "sexp/io.hpp"
#include "sexp/parser.hpp"
#include "sexp/util.hpp"
#include "sexp/value.hpp"

TEST(ParserTest, single)
{
  auto result = sexp::Parser::from_string("(1 2.5 foo \"TEXT\" bar)");
  auto expected = sexp::Value::list(sexp::Value::integer(1),
                                    sexp::Value::real(2.5),
                                    sexp::Value::symbol("foo"),
                                    sexp::Value::string("TEXT"),
                                    sexp::Value::symbol("bar"));
  ASSERT_EQ(expected, result);
}

TEST(ParserTest, single_fail)
{
  std::istringstream in("(1 2.5 foo \"TEXT\" bar bar) 5");
  ASSERT_THROW({
      sexp::Value cons = sexp::Parser::from_stream(in);
    },
    std::runtime_error);
}

TEST(ParserTest, parse_many)
{
  std::istringstream in("1 2.5 foo \"TEXT\" bar");
  std::vector<sexp::Value> value = sexp::Parser::from_stream_many(in);
  ASSERT_EQ(5, value.size());
  ASSERT_EQ(value[0].get_type(), sexp::Value::Type::INTEGER);
  ASSERT_EQ(value[1].get_type(), sexp::Value::Type::REAL);
  ASSERT_EQ(value[2].get_type(), sexp::Value::Type::SYMBOL);
  ASSERT_EQ(value[3].get_type(), sexp::Value::Type::STRING);
  ASSERT_EQ(value[4].get_type(), sexp::Value::Type::SYMBOL);
}

TEST(ParserTest, parse_positive_integer)
{
  auto sx = sexp::Parser::from_string("12345");
  ASSERT_EQ(12345, sx.as_int());
}

TEST(ParserTest, parse_negative_integer)
{
  {
    auto sx = sexp::Parser::from_string("-12345");
    ASSERT_EQ(-12345, sx.as_int());
  }

  {
    sexp::Value sx = sexp::Parser::from_string("-");
    ASSERT_TRUE(sx.is_symbol());
  }
}

TEST(ParserTest, parse_positive_real)
{
  {
    auto sx = sexp::Parser::from_string("0.125");
    ASSERT_EQ(sexp::Value::Type::REAL, sx.get_type());
    ASSERT_EQ(0.125F, sx.as_float());
  }

  {
    auto sx = sexp::Parser::from_string(".125");
    ASSERT_EQ(sexp::Value::Type::REAL, sx.get_type());
    ASSERT_EQ(0.125F, sx.as_float());
  }
}

TEST(ParserTest, parse_negative_real)
{
  {
    auto sx = sexp::Parser::from_string("-0.125");
    ASSERT_EQ(sexp::Value::Type::REAL, sx.get_type());
    ASSERT_EQ(-0.125F, sx.as_float());
  }

  {
    auto sx = sexp::Parser::from_string("-.125");
    ASSERT_EQ(sexp::Value::Type::REAL, sx.get_type());
    ASSERT_EQ(-0.125F, sx.as_float());
  }
}

TEST(ParserTest, parse_scientific_real)
{
  {
    auto sx = sexp::Parser::from_string("1.2345e-13");
    ASSERT_EQ(sexp::Value::Type::REAL, sx.get_type());
    ASSERT_EQ(1.2345e-13F, sx.as_float());
  }

  {
    auto sx = sexp::Parser::from_string("-1.2345e+13");
    ASSERT_EQ(sexp::Value::Type::REAL, sx.get_type());
    ASSERT_EQ(-1.2345e+13F, sx.as_float());
  }
}

TEST(ParserTest, parse_string)
{
  {
    auto sx = sexp::Parser::from_string("\"Hello\\nWorld\"");
    ASSERT_EQ(sexp::Value::Type::STRING, sx.get_type());
    ASSERT_EQ("Hello\nWorld", sx.as_string());
  }

  {
    auto sx = sexp::Parser::from_string("\"\\\"Hello\\nWorld\\\"\"");
    ASSERT_EQ(sexp::Value::Type::STRING, sx.get_type());
    ASSERT_EQ("\"Hello\nWorld\"", sx.as_string());
  }
}

TEST(ParserTest, parse_symbol)
{
  {
    auto sx = sexp::Parser::from_string("HelloWorld");
    ASSERT_EQ(sexp::Value::Type::SYMBOL, sx.get_type());
    ASSERT_EQ("HelloWorld", sx.as_string());
  }

  {
    auto sx = sexp::Parser::from_string("5.6.7");
    ASSERT_EQ(sexp::Value::Type::SYMBOL, sx.get_type());
    ASSERT_EQ("5.6.7", sx.as_string());
  }
}

TEST(ParserTest, parse_array)
{
  char const* sx_str = "#(1 \"foo\" #(bar))";
  auto sx = sexp::Parser::from_string(sx_str);
  ASSERT_TRUE(sx.is_array());
  ASSERT_EQ(sexp::Value::Type::ARRAY, sx.get_type());
  ASSERT_EQ(sx_str, sx.str());
}

// FIXME: Compare data structure or use simple strings?!
// "(foo . bar)" as string is ambigous in the current parser as . can be handled as symbol, not pair
TEST(ParserTest, simple_pair)
{
  sexp::Value sx = sexp::Parser::from_string("(foo . bar)");
  ASSERT_EQ("(foo . bar)", sx.str());
  ASSERT_EQ("foo", sx.get_car().as_string());
  ASSERT_EQ("bar", sx.get_cdr().as_string());
  ASSERT_EQ(sexp::Value::Type::CONS, sx.get_type());
  ASSERT_EQ(sexp::Value::Type::SYMBOL, sx.get_car().get_type());
  ASSERT_EQ(sexp::Value::Type::SYMBOL, sx.get_cdr().get_type());

  ASSERT_EQ("(foo . bar)", sexp::Parser::from_string("(foo . bar)").str());
}

TEST(ParserTest, list_pair)
{
  sexp::Value sx = sexp::Parser::from_string("(1 2 3 4 5 . 6)");
  ASSERT_EQ("(1 2 3 4 5 . 6)", sx.str());
}

TEST(ParserTest, line_numbers)
{
  sexp::Value sx = sexp::Parser::from_string("("
                                             "line1\n"
                                             "line2\n"
                                             "line3\n"
                                             ")\n");

  ASSERT_EQ(1, sexp::list_ref(sx, 0).get_line());
  ASSERT_EQ(2, sexp::list_ref(sx, 1).get_line());
  ASSERT_EQ(3, sexp::list_ref(sx, 2).get_line());
}

// C++ locale support comes in the form of ugly global state that
// spreads over most string formating functions, changing locale can
// break a lot of stuff.
class ParserLocaleTest : public ::testing::Test
{
private:
  std::locale m_oldlocale;

protected:
  ParserLocaleTest() : m_oldlocale()
  {}

  virtual void SetUp() override
  {
    try {
      // This will fail when the locale is not installed on the system,
      // just ignore the test it in that case and let the test succeed.
      std::locale::global(std::locale("de_DE.UTF-8"));
    } catch (std::exception const& err) {
      std::cerr << "warning: failed to setup locale: " << err.what() << std::endl;
    }
  }

  virtual void TearDown() override
  {
    std::locale::global(m_oldlocale);
  }
};

TEST_F(ParserLocaleTest, locale_safe_input)
{
  sexp::Value sx = sexp::Parser::from_string("0.015625");
  ASSERT_EQ(0.015625F, sx.as_float());
}

TEST_F(ParserLocaleTest, locale_safe_output)
{
  sexp::Value sx = sexp::Value::real(0.015625f);
  ASSERT_EQ("0.015625", sx.str());
}

/* EOF */
