//  $Id: PLTParser.hh,v 1.2 2000/02/09 21:43:40 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef PLTPARSER_HH
#define PLTPARSER_HH

#include <fstream>
#include <string>
#include <vector>

#include "ResDescriptor.hh"

struct PLTEOF {};

class PLTParser
{
private:
  ifstream in;
  int lineno;
  bool in_header;

  vector<string> levels;
  string theme_name;
  string theme_description;
  string theme_image;
  string background_image;
public:
  PLTParser();
  ~PLTParser();

  void   parse(string);
  vector<string> get_levels();
  string get_name();
  string get_description();
  string get_surface();
  ResDescriptor get_background();

private:
  char   get_char();
  void   expect(char);
  void   jump_spaces();
  string get_string();
  void   check_newline();
  string get_line();
  void   add_pair(string, string);
};

#endif

/* EOF */
