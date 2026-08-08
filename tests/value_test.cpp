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

#include <sstream>
#include <stdint.h>

#include "sexp/value.hpp"
#include "sexp/parser.hpp"
#include "sexp/io.hpp"

TEST(ValueTest, construct_boolean)
{
  auto sx_bool = sexp::Value::boolean(true);
  ASSERT_EQ(true, sx_bool.as_bool());
}

TEST(ValueTest, construct_integer)
{
  auto sx = sexp::Value::integer(12345789);
  ASSERT_EQ(12345789, sx.as_int());
}

TEST(ValueTest, construct_real)
{
  auto sx = sexp::Value::real(12345.0F);
  ASSERT_EQ(12345.0F, sx.as_float());
}

TEST(ValueTest, construct_symbol)
{
  auto sx = sexp::Value::symbol("Symbol");
  ASSERT_EQ("Symbol", sx.as_string());
}

TEST(ValueTest, construct_string)
{
  auto sx = sexp::Value::string("HelloWorld");
  ASSERT_EQ("HelloWorld", sx.as_string());
}

TEST(ValueTest, construct_array)
{
  auto sx = sexp::Value::array(sexp::Value::integer(1),
                               sexp::Value::integer(2),
                               sexp::Value::integer(3),
                               sexp::Value::integer(4));
  ASSERT_EQ("#(1 2 3 4)", sx.str());
  sx.append(sexp::Value::integer(5));
  ASSERT_EQ("#(1 2 3 4 5)", sx.str());
}

TEST(ValueTest, construct_cons)
{
  auto sx_integer = sexp::Value::integer(12345789);
  auto sx_cons = sexp::Value::cons(sexp::Value::integer(5), sexp::Value::nil());
  auto sx_cons2 = sexp::Value::cons(std::move(sx_integer), sexp::Value::nil());

  ASSERT_EQ(12345789, sx_cons2.get_car().as_int());
  ASSERT_EQ(sexp::Value::nil(), sx_cons2.get_cdr());
}

TEST(ValueTest, copy)
{
  sexp::Value sx = sexp::Parser::from_string("(a-symbol #f #t 1 2 3 (4 5 (6 7 8) (9 . 10) \"Hello world\"))");
  sexp::Value sx_copy = sx; // NOLINT
  ASSERT_EQ(sx.str(), sx_copy.str());
}

TEST(ValueTest, equal)
{
  sexp::Value lhs = sexp::Parser::from_string("(a-symbol #f #t 1 2 3 (4 5 (6 7 8) (9 . 10) \"Hello world\"))");
  sexp::Value rhs = sexp::Parser::from_string("(a-symbol #f #t 1 2 3 (4 5 (6 7 8) (9 . 10) \"Hello world\"))");
  ASSERT_TRUE(lhs == rhs);
}

TEST(ValueTest, assignment)
{
  sexp::Value lhs = sexp::Parser::from_string("(a-symbol #f #t 1 2 3 (4 5 (6 7 8) (9 . 10) \"Hello world\"))");
  sexp::Value rhs = sexp::Parser::from_string("(a-symbol #f #t 1 2 3 (4 5 (6 7 8) (9 . 10) \"Hello world\"))");
  ASSERT_EQ(lhs, rhs);
  sexp::Value tmp = lhs;
  lhs = rhs;
  rhs = tmp;
  ASSERT_EQ(lhs, rhs);
}

TEST(ValueTest, type_errors_boolean)
{
  sexp::Value sx = sexp::Value::boolean(true);
  ASSERT_THROW(sx.append(sexp::Value::nil()), sexp::TypeError);
  ASSERT_THROW(sx.set_car(sexp::Value::nil()), sexp::TypeError);
  ASSERT_THROW(sx.set_cdr(sexp::Value::nil()), sexp::TypeError);
  ASSERT_THROW(sx.get_car(), sexp::TypeError);
  ASSERT_THROW(sx.get_cdr(), sexp::TypeError);

  ASSERT_THROW(sx.as_int(), sexp::TypeError);
  ASSERT_THROW(sx.as_float(), sexp::TypeError);
  ASSERT_THROW(sx.as_string(), sexp::TypeError);
}

TEST(ValueTest, type_errors_integer)
{
  sexp::Value sx = sexp::Value::integer(5);
  ASSERT_THROW(sx.append(sexp::Value::nil()), sexp::TypeError);
  ASSERT_THROW(sx.set_car(sexp::Value::nil()), sexp::TypeError);
  ASSERT_THROW(sx.set_cdr(sexp::Value::nil()), sexp::TypeError);
  ASSERT_THROW(sx.get_car(), sexp::TypeError);
  ASSERT_THROW(sx.get_cdr(), sexp::TypeError);

  ASSERT_THROW(sx.as_bool(), sexp::TypeError);
  ASSERT_THROW(sx.as_string(), sexp::TypeError);
}

TEST(ValueTest, type_errors_real)
{
  sexp::Value sx = sexp::Value::real(1.125F);
  ASSERT_THROW(sx.append(sexp::Value::nil()), sexp::TypeError);
  ASSERT_THROW(sx.set_car(sexp::Value::nil()), sexp::TypeError);
  ASSERT_THROW(sx.set_cdr(sexp::Value::nil()), sexp::TypeError);
  ASSERT_THROW(sx.get_car(), sexp::TypeError);
  ASSERT_THROW(sx.get_cdr(), sexp::TypeError);

  ASSERT_THROW(sx.as_bool(), sexp::TypeError);
  ASSERT_THROW(sx.as_string(), sexp::TypeError);
}

TEST(ValueTest, type_errors_symbol)
{
  sexp::Value sx = sexp::Value::symbol("HelloWorld");
  ASSERT_THROW(sx.append(sexp::Value::nil()), sexp::TypeError);
  ASSERT_THROW(sx.set_car(sexp::Value::nil()), sexp::TypeError);
  ASSERT_THROW(sx.set_cdr(sexp::Value::nil()), sexp::TypeError);
  ASSERT_THROW(sx.get_car(), sexp::TypeError);
  ASSERT_THROW(sx.get_cdr(), sexp::TypeError);

  ASSERT_THROW(sx.as_bool(), sexp::TypeError);
  ASSERT_THROW(sx.as_int(), sexp::TypeError);
  ASSERT_THROW(sx.as_float(), sexp::TypeError);
}

TEST(ValueTest, type_errors_string)
{
  sexp::Value sx = sexp::Value::string("HelloWorld");
  ASSERT_THROW(sx.append(sexp::Value::nil()), sexp::TypeError);
  ASSERT_THROW(sx.set_car(sexp::Value::nil()), sexp::TypeError);
  ASSERT_THROW(sx.set_cdr(sexp::Value::nil()), sexp::TypeError);
  ASSERT_THROW(sx.get_car(), sexp::TypeError);
  ASSERT_THROW(sx.get_cdr(), sexp::TypeError);

  ASSERT_THROW(sx.as_bool(), sexp::TypeError);
  ASSERT_THROW(sx.as_int(), sexp::TypeError);
  ASSERT_THROW(sx.as_float(), sexp::TypeError);
}

TEST(ValueTest, type_errors_cons)
{
  sexp::Value sx = sexp::Value::cons(sexp::Value::integer(5), sexp::Value::integer(5));
  ASSERT_THROW(sx.as_bool(), sexp::TypeError);
  ASSERT_THROW(sx.as_int(), sexp::TypeError);
  ASSERT_THROW(sx.as_float(), sexp::TypeError);
  ASSERT_THROW(sx.as_string(), sexp::TypeError);
}

TEST(ValueTest, object_size)
{
#if INTPTR_MAX == INT32_MAX
  // on 32bit systems
#  ifdef _MSC_VER
  ASSERT_EQ(12, sizeof(sexp::Value));
#  else
  ASSERT_EQ(8, sizeof(sexp::Value));
#  endif
#elif INTPTR_MAX == INT64_MAX
  // on 64bit systems
  ASSERT_EQ(16, sizeof(sexp::Value));
#else
#  error "environment is neither 32 nor 64-bit"
#endif
}

/* EOF */
