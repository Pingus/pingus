//  $Id: StringReader.hh,v 1.3 2000/02/11 16:58:28 grumbel Exp $
// 
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef STRINGREADER_HH
#define STRINGREADER_HH

#include <list>
#include <string>
#include <ClanLib/core.h>

class StringReader
{
private:
  list<std::string>* strings;
  list<std::string*> completions;
  std::string current_string;
  std::string default_string;
  bool finished;
  std::string description;
  CL_Font* font;

public:
  StringReader();
  StringReader(string, string);
  virtual ~StringReader();

  void   set_strings(std::list<std::string>*);
  std::string read_string();
  void   draw();
  std::string find_uniq();
  std::string while_eq(const string& a, const string& b);
  void   complete_string();
};

#endif

/* EOF */
