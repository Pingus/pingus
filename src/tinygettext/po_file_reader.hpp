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

#ifndef HEADER_PO_FILE_READER_HPP
#define HEADER_PO_FILE_READER_HPP

#include <string>
#include <iostream>
#include "dictionary.hpp"

namespace TinyGetText {

class POFileReader
{
private:
  struct Token
  {
    std::string keyword;
    std::string content;
  };

  Dictionary& dict;

  std::string from_charset;
  std::string to_charset;

  std::string current_msgid;
  std::string current_msgid_plural;
  std::map<int, std::string> msgstr_plural;

  int line_num;

  enum { WANT_MSGID, WANT_MSGSTR, WANT_MSGSTR_PLURAL, WANT_MSGID_PLURAL } state;

public:
  POFileReader(std::istream& in, Dictionary& dict_);

  void parse_header(const std::string& header);
  void add_token(const Token& token);  
  void tokenize_po(std::istream& in);

  inline int getchar(std::istream& in) 
  {
    int c = in.get();
    if (c == '\n')
      line_num += 1;
    return c;
  }
  
};

} // namespace TinyGetText

#endif

/* EOF */
