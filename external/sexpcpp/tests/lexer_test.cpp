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

#include "sexp/lexer.hpp"

TEST(LexerTest, simple_tokens)
{
  std::istringstream is("(foo . bar #())");
  sexp::Lexer lexer(is);
  ASSERT_EQ(sexp::Lexer::TOKEN_OPEN_PAREN, lexer.get_next_token());
  ASSERT_EQ(sexp::Lexer::TOKEN_SYMBOL, lexer.get_next_token());
  ASSERT_EQ("foo", lexer.get_string());
  ASSERT_EQ(sexp::Lexer::TOKEN_DOT, lexer.get_next_token());
  ASSERT_EQ(sexp::Lexer::TOKEN_SYMBOL, lexer.get_next_token());
  ASSERT_EQ("bar", lexer.get_string());
  ASSERT_EQ(sexp::Lexer::TOKEN_ARRAY_START, lexer.get_next_token());
  ASSERT_EQ(sexp::Lexer::TOKEN_CLOSE_PAREN, lexer.get_next_token());
  ASSERT_EQ(sexp::Lexer::TOKEN_CLOSE_PAREN, lexer.get_next_token());
  ASSERT_EQ(sexp::Lexer::TOKEN_EOF, lexer.get_next_token());
}

TEST(LexerTest, long_tokens)
{
  std::string long_token(32768, 'X');
  std::istringstream is("(" + long_token + ")");
  sexp::Lexer lexer(is);
  ASSERT_EQ(sexp::Lexer::TOKEN_OPEN_PAREN, lexer.get_next_token());
  ASSERT_EQ(sexp::Lexer::TOKEN_SYMBOL, lexer.get_next_token());
  ASSERT_EQ(long_token, lexer.get_string());
  ASSERT_EQ(sexp::Lexer::TOKEN_CLOSE_PAREN, lexer.get_next_token());
  ASSERT_EQ(sexp::Lexer::TOKEN_EOF, lexer.get_next_token());
}

TEST(LexerTest, comment)
{
  std::istringstream is(";comment\n(foo ;comment\n;comment\n bar);EOF");
  sexp::Lexer lexer(is);
  ASSERT_EQ(sexp::Lexer::TOKEN_OPEN_PAREN, lexer.get_next_token());
  ASSERT_EQ(sexp::Lexer::TOKEN_SYMBOL, lexer.get_next_token());
  ASSERT_EQ(sexp::Lexer::TOKEN_SYMBOL, lexer.get_next_token());
  ASSERT_EQ(sexp::Lexer::TOKEN_CLOSE_PAREN, lexer.get_next_token());
}

TEST(LexerTest, token_dot)
{
  std::vector<std::string> texts = {
    "."
  };

  for(const auto& text : texts)
  {
    std::istringstream is(text);
    sexp::Lexer lexer(is);
    ASSERT_EQ(sexp::Lexer::TOKEN_DOT, lexer.get_next_token());
    ASSERT_EQ(text, lexer.get_string());
  }
}

TEST(LexerTest, token_symbol)
{
  std::vector<std::string> texts = {
    "SymbolTest",
    "foo-bar",
    "1.2.3",
    "e50",
  };

  for(const auto& text : texts)
  {
    std::istringstream is(text);
    sexp::Lexer lexer(is);
    ASSERT_EQ(sexp::Lexer::TOKEN_SYMBOL, lexer.get_next_token());
    ASSERT_EQ(text, lexer.get_string());
  }
}

TEST(LexerTest, token_string)
{
  std::istringstream is("\"StringTest\"");
  sexp::Lexer lexer(is);
  ASSERT_EQ(sexp::Lexer::TOKEN_STRING, lexer.get_next_token());
  ASSERT_EQ("StringTest", lexer.get_string());
}

TEST(LexerTest, token_integer)
{
  std::vector<std::string> texts = {
    "123456789",
    "-123456789"
  };

  for(const auto& text : texts)
  {
    std::istringstream is(text);
    sexp::Lexer lexer(is);
    ASSERT_EQ(sexp::Lexer::TOKEN_INTEGER, lexer.get_next_token());
    ASSERT_EQ(text, lexer.get_string());
  }
}

TEST(LexerTest, token_real)
{
  std::vector<std::string> texts = {
    ".1234",
    ".1234e15",
    "1234.6789",
    "1234.",
    "1234.5678",
    "1234.5678e15",
    "-1234.5678e15",
    "1234.5678e15",
    "1234.5678E+15",
    "-1234.5678E-15",
  };

  for(const auto& text : texts)
  {
    std::istringstream is(text);
    sexp::Lexer lexer(is);
    ASSERT_EQ(sexp::Lexer::TOKEN_REAL, lexer.get_next_token());
    ASSERT_EQ(text, lexer.get_string());
  }
}

/* EOF */
