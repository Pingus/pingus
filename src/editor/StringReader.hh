//  $Id: StringReader.hh,v 1.2 2000/02/09 21:43:43 grumbel Exp $
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
  list<string>* strings;
  list<string*> completions;
  string current_string;
  string default_string;
  bool finished;
  string description;
  CL_Font* font;

public:
  StringReader();
  StringReader(string, string);
  virtual ~StringReader();

  void   set_strings(list<string>*);
  string read_string();
  void   draw();
  string find_uniq();
  string while_eq(const string& a, const string& b);
  void   complete_string();
};

#endif

/* EOF */
