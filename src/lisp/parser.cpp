
//  TuxKart - a fun racing game with go-kart
//  Copyright (C) 2004 Matthias Braun <matze@braunis.de>
//  code in this file based on lispreader from Mark Probst
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

#include <fstream>
#include <sstream>
#include <string.h>

#include "lisp.hpp"
#include "parser.hpp"

namespace lisp
{

class ParseError : public std::exception
{
public:
  ParseError(const Parser* parser, const std::string& message) throw() :
    string()
  {
    std::ostringstream msg;
    msg << "Parse error in file '" << parser->filename << "' line "
        << parser->lexer->getLineNumber() << ": " << message;
    string = msg.str();
  }
  ~ParseError() throw()
  {}

  const char* what() const throw()
  {
    return string.c_str();
  }

private:
  std::string string;
};

Parser::Parser() :
  filename(),
  lexer(0),
  token()
{
}

Parser::~Parser()
{
  delete lexer;
}

std::shared_ptr<Lisp>
Parser::parse(const std::string& filename)
{
  std::ifstream in(filename.c_str()); // FIXME: Temporary switched away from physfs IFileStream
  if(!in.good()) {
    std::stringstream msg;
    msg << "Parser problem: Couldn't open file '" << filename << "'.";
    throw std::runtime_error(msg.str());
  }

  return parse(in, filename);
}

std::shared_ptr<Lisp>
Parser::parse(std::istream& stream, const std::string& filename)
{
  std::unique_ptr<Parser> parser (new Parser());

  parser->filename = filename;
  parser->lexer = new Lexer(stream);

  parser->token = parser->lexer->getNextToken();
  if(parser->token != Lexer::TOKEN_OPEN_PAREN)
    throw ParseError(parser.get(), "file doesn't start with '('");

  std::shared_ptr<Lisp> result(parser->parse());
  if(parser->token != Lexer::TOKEN_EOF) {
    if(parser->token == Lexer::TOKEN_CLOSE_PAREN)
      throw ParseError(parser.get(), "too many ')'");
    else
      throw ParseError(parser.get(), "extra tokens at end of file");
  }
    
  return result;
}

std::shared_ptr<Lisp>
Parser::parse()
{
  std::vector<std::shared_ptr<Lisp> > entries;
  while(token != Lexer::TOKEN_CLOSE_PAREN && token != Lexer::TOKEN_EOF) {
    switch(token) {
      case Lexer::TOKEN_OPEN_PAREN:
        token = lexer->getNextToken();
      
        // Handle (_ "blup") strings that need to be translated
        if(token == Lexer::TOKEN_SYMBOL
           && strcmp(lexer->getString(), "_") == 0) {
          token = lexer->getNextToken();
          if(token != Lexer::TOKEN_STRING)
            throw ParseError(this, "Expected string after '(_' sequence");
          entries.push_back(std::shared_ptr<Lisp>(new Lisp(Lisp::TYPE_STRING, lexer->getString())));
          
          token = lexer->getNextToken();
          if(token != Lexer::TOKEN_CLOSE_PAREN)
            throw ParseError(this, "Expected ')' after '(_ ""' sequence");
          break;
        }
      
        entries.push_back(parse());
        if(token != Lexer::TOKEN_CLOSE_PAREN) {
          if(token == Lexer::TOKEN_EOF)
            throw ParseError(this, "Expected ')' token, got EOF");
          else
            throw ParseError(this, "Expected ')' token");
        }
        break;
      case Lexer::TOKEN_SYMBOL:
        entries.push_back(std::shared_ptr<Lisp>(new Lisp(Lisp::TYPE_SYMBOL, lexer->getString())));
        break;
      case Lexer::TOKEN_STRING:
        entries.push_back(std::shared_ptr<Lisp>(new Lisp(Lisp::TYPE_STRING, lexer->getString())));
        break;
      case Lexer::TOKEN_INTEGER: {
        int val;
        sscanf(lexer->getString(), "%d", &val);
        entries.push_back(std::shared_ptr<Lisp>(new Lisp(val)));
        break;
      }
      case Lexer::TOKEN_REAL: {
        float val;
        sscanf(lexer->getString(), "%f", &val);
        entries.push_back(std::shared_ptr<Lisp>(new Lisp(val)));
        break;
      }
      case Lexer::TOKEN_TRUE:
        entries.push_back(std::shared_ptr<Lisp>(new Lisp(true)));
        break;
      case Lexer::TOKEN_FALSE:
        entries.push_back(std::shared_ptr<Lisp>(new Lisp(false)));
        break;
      default:
        // this should never happen
        assert(false);
    }

    token = lexer->getNextToken();
  }
  
  return std::shared_ptr<Lisp>(new Lisp(entries));
}

} // end of namespace lisp
