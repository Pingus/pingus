//  $Id: string_tokenizer.hxx,v 1.2 2003/04/14 19:40:41 grumbel Exp $
// 
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_STRING_TOKENIZER_HXX
#define HEADER_PINGUS_STRING_TOKENIZER_HXX

#include <string>
#include <vector>

/** */
class StringTokenizer
{
private:
  typedef std::vector<std::string> Token;
  Token tokens;
public:
  typedef Token::const_iterator iterator;

  /** Splits a string at \a split_char, empty tokens are ignored */
  StringTokenizer(const std::string& arg_str, char split_char);

  iterator begin() const { return tokens.begin(); }
  iterator end() const { return tokens.end(); }
private:
  StringTokenizer (const StringTokenizer&);
  StringTokenizer& operator= (const StringTokenizer&);
};

#endif

/* EOF */
