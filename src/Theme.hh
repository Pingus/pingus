//  $Id: Theme.hh,v 1.7 2000/06/20 20:32:12 grumbel Exp $
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

#include "MultiLineText.hh"
#include "PLTParser.hh"

///
class Theme
{
private:
  ///
  PLTParser plt;
  ///
  CL_Surface* surface;
  ///
  CL_Surface* background;
  ///
  CL_Font*    font;
  ///
  CL_Font*    title;
  ///
  int      accessible_levels;
  ///
  int      current_level;
  ///
  std::vector<std::string> levels;
  ///
  std::vector<std::string> levelnames;
  ///
  std::string   status_file;

  /** This is the position at which the levelnames are printed, it is
      used to calculate mark_level_at_point() */
  int level_start_y_pos;

  ///
  bool has_description;
  ///
  MultiLineText description;
public:
  ///
  Theme();
  ///
  Theme(std::string);
  ///
  ~Theme();

  /** Marks the level, which is under the given coordinates 
      @param x X-Position (normaly CL_Mouse::get_x())
      @param y Y-Position (normaly CL_Mouse::get_y()) */
  void mark_level_at_point(int x, int y);

  ///
  void next_level();
  ///
  void previous_level();
  ///
  void load(std::string);
  ///
  void load_status(std::string);
  ///
  void load_levels();
  ///
  void draw_title();
  ///
  void play();
};

#endif

/* EOF */

