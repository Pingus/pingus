//  $Id: Theme.hh,v 1.3 2000/02/11 16:58:26 grumbel Exp $
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
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
//  02111-1307, USA. 

#ifndef THEME_HH
#define THEME_HH

#include <string>
#include <vector>
#include <ClanLib/core.h>


#include "PLTParser.hh"

class Theme
{
private:
  PLTParser plt;
  CL_Surface* surface;
  CL_Surface* background;
  CL_Font*    font;
  CL_Font*    title;
  int      accessible_levels;
  int      current_level;
  vector<std::string> levels;
  vector<std::string> levelnames;
  std::string   status_file;
public:
  Theme();
  Theme(string);
  ~Theme();
  
  void next_level();
  void previous_level();
  void load(string);
  void load_status(string);
  void load_levels();
  void draw_title();
  void play();
};

#endif

/* EOF */

