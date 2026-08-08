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

#ifndef HEADER_SEXP_PARSER_HPP
#define HEADER_SEXP_PARSER_HPP

#include <memory>
#include <vector>

#include <sexp/lexer.hpp>
#include <sexp/value.hpp>

namespace sexp {

class Lexer;

class Parser
{
public:
  enum { USE_ARRAYS = true };

  static Value from_string(std::string const& str, bool use_arrays = false);
  static Value from_stream(std::istream& stream, bool use_arrays = false);

  static std::vector<Value> from_string_many(std::string const& str, bool use_arrays =  false);
  static std::vector<Value> from_stream_many(std::istream& stream, bool use_arrays = false);

public:
  Parser(Lexer& lexer);
  ~Parser();

private:
  [[noreturn]]
  void parse_error(const char* msg) const;
  std::vector<Value> read_many();
  Value read();

private:
  Lexer& m_lexer;
  Lexer::TokenType m_token;

private:
  Parser(const Parser&);
  Parser & operator=(const Parser&);
};

} // namespace sexp

#endif

/* EOF */
