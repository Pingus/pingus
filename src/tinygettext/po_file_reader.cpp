//  $Id$
// 
//  TinyGetText - A small flexible gettext() replacement
//  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
// 
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include <config.h>
#include <vector>
#include <iconv.h>
#include <errno.h>
#include <sstream>
#include <stdexcept>
#include "iconv.hpp"
#include "po_file_reader.hpp"

namespace TinyGetText {

static bool has_prefix(const std::string& lhs, const std::string rhs)
{
  if (lhs.length() < rhs.length())
    return false;
  else
    return lhs.compare(0, rhs.length(), rhs) == 0;
}

POFileReader::POFileReader(std::istream& in, Dictionary& dict_)
  : dict(dict_)
{
  state = WANT_MSGID;
  line_num = 0;
  tokenize_po(in);

  if (dict.get_charset().empty())
    {
      std::cout << "POFileReader: Error: charset not declared in .po file, fallback to ISO-8859-1" << std::endl;
      dict.set_charset("ISO-8859-1");
    }
}

void
POFileReader::parse_header(const std::string& header)
{
  // Seperate the header in lines
  typedef std::vector<std::string> Lines;
  Lines lines;
    
  std::string::size_type start = 0;
  for(std::string::size_type i = 0; i < header.length(); ++i)
    {
      if (header[i] == '\n')
        {
          lines.push_back(header.substr(start, i - start));
          start = i+1;
        }
    }

  for(Lines::iterator i = lines.begin(); i != lines.end(); ++i)
    {
      if (has_prefix(*i, "Content-Type: text/plain; charset=")) {
        dict.set_charset(i->substr(strlen("Content-Type: text/plain; charset=")));
        break;
      }
    }  
}

void
POFileReader::add_token(const Token& token)
{
  switch(state) 
    {
    case WANT_MSGID:
      if (token.keyword == "msgid") 
        {
          current_msgid = token.content;
          state = WANT_MSGID_PLURAL;
        }
      else if (token.keyword.empty())
        {
          //std::cerr << "Got EOF, everything looks ok." << std::endl;
        }
      else
        {
          std::cerr << "tinygettext: expected 'msgid' keyword, got '" << token.keyword 
                    << "' at line " << line_num << std::endl;
        }
      break;
    
    case WANT_MSGID_PLURAL:
      if (token.keyword == "msgid_plural") 
        {
          current_msgid_plural = token.content;
          state = WANT_MSGSTR_PLURAL;
        } 
      else
        {
          state = WANT_MSGSTR;
          add_token(token);
        }
      break;

    case WANT_MSGSTR:
      if (token.keyword == "msgstr") 
        {
          if (current_msgid == "") 
            { // .po Header is hidden in the msgid with the empty string
              parse_header(token.content);
            }
          else
            {
              dict.add_translation(current_msgid, token.content);
            }
          state = WANT_MSGID;
        } 
      else
        {
          std::cerr << "tinygettext: expected 'msgstr' keyword, got " << token.keyword 
                    << " at line " << line_num << std::endl;
        }
      break;

    case WANT_MSGSTR_PLURAL:
      if (has_prefix(token.keyword, "msgstr[")) 
        {
          int num;
          if (sscanf(token.keyword.c_str(), "msgstr[%d]", &num) != 1) 
            {
              std::cerr << "Error: Couldn't parse: " << token.keyword << std::endl;
            } 
          else 
            {
              msgstr_plural[num] = token.content;
            }
        }
      else 
        {
          dict.add_translation(current_msgid, current_msgid_plural, msgstr_plural);

          state = WANT_MSGID;
          add_token(token);
        }
      break;
    }
}
  
void
POFileReader::tokenize_po(std::istream& in)
{
  enum State { READ_KEYWORD, 
               READ_CONTENT,
               READ_CONTENT_IN_STRING,
               SKIP_COMMENT };

  State state = READ_KEYWORD;
  int c;
  Token token;

  while((c = getchar(in)) != EOF)
    {
      //std::cout << "Lexing char: '" << char(c) << "' " << c << " state: " << state << std::endl;
      switch(state)
        {
        case READ_KEYWORD:
          if (c == '#')
            {
              state = SKIP_COMMENT;
            }
          else if (isspace(c))
            {
              state = READ_KEYWORD;
            }
          else
            {
              // Read a new token
              token = Token();
                
              do { // Read keyword 
                token.keyword += c;
              } while((c = getchar(in)) != EOF && !isspace(c));
              in.unget();

              state = READ_CONTENT;
            }
          break;

        case READ_CONTENT:
          while((c = getchar(in)) != EOF)
            {
              if (c == '"') { 
                // Found start of content
                state = READ_CONTENT_IN_STRING;
                break;
              } else if (isspace(c)) {
                // skip
              } else { // Read something that may be a keyword
                in.unget();
                state = READ_KEYWORD;
                add_token(token);
                break;
              }
            }
          break;

        case READ_CONTENT_IN_STRING:
          if (c == '\\') {
            c = getchar(in);
            if (c != EOF)
              {
                if (c == 'n') token.content += '\n';
                else if (c == 't') token.content += '\t';
                else if (c == 'r') token.content += '\r';
                else if (c == '"') token.content += '"';
                else
                  {
                    std::cout << "Unhandled escape character: " << char(c) << std::endl;
                  }
              }
            else
              {
                std::cout << "Unterminated string" << std::endl;
              }
          } else if (c == '"') { // Content string is terminated
            state = READ_CONTENT;
          } else {
            token.content += c;
          }
          break;

        case SKIP_COMMENT:
          if (c == '\n')
            state = READ_KEYWORD;
          break;
        }
    }
  // add_token(token);
}

} // namespace TinyGetText

/* EOF */
