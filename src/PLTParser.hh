//  $Id: PLTParser.hh,v 1.4 2000/04/24 13:15:41 grumbel Exp $
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
  std::ifstream in;
  int lineno;
  bool in_header;

  std::vector<std::string> levels;
  std::string theme_name;
  std::string theme_description;
  std::string theme_image;
  std::string background_image;
public:
  PLTParser();
  ~PLTParser();

  void   parse(std::string);
  std::vector<std::string> get_levels();
  std::string get_name();
  std::string get_description();
  std::string get_surface();
  ResDescriptor get_background();

private:
  char   get_char();
  void   expect(char);
  void   jump_spaces();
  std::string get_string();
  void   check_newline();
  std::string get_line();
  void   add_pair(std::string, std::string);
};

#endif

/* EOF */
