// SExp - A S-Expression Parser for C++
// Copyright (C) 2006 Matthias Braun <matze@braunis.de>
//               2015 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_SEXP_LEXER_HPP
#define HEADER_SEXP_LEXER_HPP

#include <istream>

namespace sexp {

class Lexer
{
public:
  enum TokenType {
    TOKEN_EOF,
    TOKEN_OPEN_PAREN,
    TOKEN_CLOSE_PAREN,
    TOKEN_DOT,
    TOKEN_SYMBOL,
    TOKEN_STRING,
    TOKEN_INTEGER,
    TOKEN_REAL,
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_ARRAY_START
  };

public:
  Lexer(std::istream& stream, bool use_arrays = false);
  ~Lexer();

  TokenType get_next_token();
  std::string const& get_string() const { return m_token_string; }
  int get_line_number() const { return m_linenumber; }

private:
  static const int MAX_TOKEN_LENGTH = 16384;
  static const int BUFFER_SIZE = 16384;

private:
  inline void next_char();
  inline void add_char();

private:
  std::istream& m_stream;
  bool m_use_arrays;
  bool m_eof;
  int m_linenumber;
  char m_buffer[BUFFER_SIZE+1];
  char* m_bufend;
  char* m_bufpos;
  int m_c;
  std::string m_token_string;

private:
  Lexer(const Lexer&);
  Lexer & operator=(const Lexer&);
};

} // namespace sexp

#endif

/* EOF */
