
//  TuxKart - a fun racing game with go-kart
//  Copyright (C) 2004 Matthias Braun <matze@braunis.de>
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
#ifndef __LISPPARSER_H__
#define __LISPPARSER_H__

#include <memory>
#include <string>

#include "lexer.hpp"

namespace lisp
{

class Lisp;

class Parser
{
public:
  ~Parser();
  static std::shared_ptr<Lisp> parse(const std::string& filename);
  static std::shared_ptr<Lisp> parse(std::istream& stream, const std::string& filename = "");

private:
  friend class ParseError;

  Parser();
  
  std::shared_ptr<Lisp> parse();
  
  std::string filename;
  Lexer* lexer;
  Lexer::TokenType token;

private:
  Parser(const Parser&);
  Parser & operator=(const Parser&);
};

} // end of namespace lisp

#endif

