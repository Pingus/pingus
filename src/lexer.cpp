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

#include "sexp/lexer.hpp"

#include <assert.h>
#include <string.h>
#include <sstream>
#include <stdexcept>
#include <stdio.h>

namespace sexp {

Lexer::Lexer(std::istream& newstream, bool use_arrays) :
  m_stream(newstream),
  m_use_arrays(use_arrays),
  m_eof(false),
  m_linenumber(0),
  m_bufend(),
  m_bufpos(),
  m_c(),
  m_token_string()
{
  // trigger a refill of the buffer
  m_bufpos = nullptr;
  m_bufend = nullptr;
  next_char();
}

Lexer::~Lexer()
{
}

void
Lexer::next_char()
{
  if (m_bufpos >= m_bufend || (m_bufpos == nullptr && m_bufend == nullptr) /* Initial refill trigger */) {
    if (m_eof) {
      m_c = EOF;
      return;
    }
    m_stream.read(m_buffer, BUFFER_SIZE);
    std::streamsize bytes_read = m_stream.gcount();

    m_bufpos = m_buffer;
    m_bufend = m_buffer + bytes_read;

    // the following is a hack that appends an additional ' ' at the end of
    // the file to avoid problems when parsing symbols/elements and a sudden
    // EOF. This is faster than relying on unget and IMO also nicer.
    if (bytes_read == 0 || m_stream.eof()) {
      m_eof = true;
      *m_bufend = ' ';
      ++m_bufend;
    }
  }

  if (m_bufpos == nullptr) {
    return;
  }

  m_c = static_cast<unsigned char>(*m_bufpos++);
  if (m_c == '\n') {
    ++m_linenumber;
  }
}

void
Lexer::add_char()
{
  m_token_string += static_cast<char>(m_c);
  next_char();
}

Lexer::TokenType
Lexer::get_next_token()
{
  static const char* delims = "\"();";

  while(isspace(m_c)) {
    next_char();
  }

  m_token_string.clear();

  switch(m_c)
  {
    case ';': // comment
      while(m_c != '\n') {
        next_char();
      }
      return get_next_token(); // and again

    case '(':
      next_char();
      if (m_use_arrays)
      {
        return TOKEN_ARRAY_START;
      }
      else
      {
        return TOKEN_OPEN_PAREN;
      }
    case ')':
      next_char();
      return TOKEN_CLOSE_PAREN;

    case '"': {  // string
      int startline = m_linenumber;
      while(1) {
        next_char();
        switch(m_c) {
          case '"':
            next_char();
            goto string_finished;
          case '\r':
            continue;
          case '\n':
            break;
          case '\\':
            next_char();
            switch(m_c) {
              case 'n':
                m_c = '\n';
                break;
              case 't':
                m_c = '\t';
                break;
            }
            break;
          case EOF: {
            std::stringstream msg;
            msg << "Parse error in line " << startline << ": "
                << "EOF while parsing string.";
            throw std::runtime_error(msg.str());
          }
          default:
            break;
        }
        m_token_string += static_cast<char>(m_c);
      }
      string_finished:
      return TOKEN_STRING;
    }
    case '#': // constant
      next_char();

      if (m_c == '(')
      {
        next_char();
        return TOKEN_ARRAY_START;
      }
      else
      {
        while(isalnum(m_c) || m_c == '_') {
          add_char();
        }

        if (m_token_string == "t")
        {
          return TOKEN_TRUE;
        }
        else if (m_token_string == "f")
        {
          return TOKEN_FALSE;
        }
        else
        {
          // we only handle #t and #f constants at the moment...
          std::stringstream msg;
          msg << "Parse Error in line " << m_linenumber << ": "
              << "Unknown constant '" << m_token_string << "'.";
          throw std::runtime_error(msg.str());
        }
      }

    case EOF:
      return TOKEN_EOF;

    default:
      {
        enum {
          STATE_INIT,
          STATE_SYMBOL,
          STATE_MAYBE_DOT,
          STATE_MAYBE_INTEGER_SIGN,
          STATE_MAYBE_INTEGER_PART,
          STATE_MAYBE_FRACTIONAL_START,
          STATE_MAYBE_FRACTIONAL_PART,
          STATE_MAYBE_EXPONENT_SIGN,
          STATE_MAYBE_EXPONENT_START,
          STATE_MAYBE_EXPONENT_PART,
        } state = STATE_INIT;

        bool has_integer_part = false;
        bool has_fractional_part = false;
        do
        {
          switch(state)
          {
            case STATE_INIT:
              if (isdigit(m_c)) {
                has_integer_part = true;
                state = STATE_MAYBE_INTEGER_PART;
              } else if (m_c == '-' || m_c == '+') {
                state = STATE_MAYBE_INTEGER_SIGN;
              } else if (m_c == '.') {
                state = STATE_MAYBE_DOT;
              } else {
                state = STATE_SYMBOL;
              }
              break;

            case STATE_SYMBOL:
              break;

            case STATE_MAYBE_DOT:
              if (isdigit(m_c)) {
                state = STATE_MAYBE_FRACTIONAL_START;
              } else {
                state = STATE_SYMBOL;
              }
              break;

            case STATE_MAYBE_INTEGER_SIGN:
              if (isdigit(m_c)) {
                has_integer_part = true;
                state = STATE_MAYBE_INTEGER_PART;
              } else if (m_c == '.') {
                state = STATE_MAYBE_FRACTIONAL_START;
              }
              break;

            case STATE_MAYBE_INTEGER_PART:
              if (isdigit(m_c)) {
                // loop
              } else if (m_c == '.') {
                state = STATE_MAYBE_FRACTIONAL_START;
              } else if (m_c == 'e' || m_c == 'E') {
                state = STATE_MAYBE_EXPONENT_SIGN;
              } else {
                state = STATE_SYMBOL;
              }
              break;

            case STATE_MAYBE_FRACTIONAL_START:
              if (isdigit(m_c)) {
                has_fractional_part = true;
                state = STATE_MAYBE_FRACTIONAL_PART;
              } else {
                state = STATE_SYMBOL;
              }
              break;

            case STATE_MAYBE_FRACTIONAL_PART:
              if (isdigit(m_c)) {
                // loop
              } else if ((has_integer_part || has_fractional_part) &&
                         (m_c == 'e' || m_c == 'E')) {
                state = STATE_MAYBE_EXPONENT_SIGN;
              } else {
                state = STATE_SYMBOL;
              }
              break;

            case STATE_MAYBE_EXPONENT_SIGN:
              if (m_c == '-' || m_c == '+') {
                state = STATE_MAYBE_EXPONENT_START;
              } else if (isdigit(m_c)) {
                state = STATE_MAYBE_EXPONENT_PART;
              } else {
                state = STATE_SYMBOL;
              }
              break;

            case STATE_MAYBE_EXPONENT_START:
              if (isdigit(m_c)) {
                state = STATE_MAYBE_EXPONENT_PART;
              } else {
                state = STATE_SYMBOL;
              }
              break;

            case STATE_MAYBE_EXPONENT_PART:
              if (isdigit(m_c)) {
                // loop
              } else {
                state = STATE_SYMBOL;
              }
              break;
          }

          add_char();
        }
        while(!isspace(m_c) && !strchr(delims, m_c));

        switch(state)
        {
          case STATE_INIT:
            assert(false && "never reached");
            return TOKEN_EOF;

          case STATE_SYMBOL:
            return TOKEN_SYMBOL;

          case STATE_MAYBE_DOT:
            return TOKEN_DOT;

          case STATE_MAYBE_INTEGER_SIGN:
            return TOKEN_SYMBOL;

          case STATE_MAYBE_INTEGER_PART:
            return TOKEN_INTEGER;

          case STATE_MAYBE_FRACTIONAL_START:
            if (has_integer_part) {
              return TOKEN_REAL;
            } else {
              return TOKEN_SYMBOL;
            }

          case STATE_MAYBE_FRACTIONAL_PART:
            return TOKEN_REAL;

          case STATE_MAYBE_EXPONENT_SIGN:
          case STATE_MAYBE_EXPONENT_START:
            return TOKEN_SYMBOL;

          case STATE_MAYBE_EXPONENT_PART:
            return TOKEN_REAL;
        }
      }
      assert(false && "never reached");
      return TOKEN_EOF;
  }
}

} // namespace sexp

/* EOF */
