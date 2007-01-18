//  $Id: theme.hxx,v 1.9 2003/10/21 21:37:06 grumbel Exp $
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

#ifndef HEADER_PINGUS_THEME_HXX
#define HEADER_PINGUS_THEME_HXX

#include <ClanLib/Display/surface.h>
#include "multiline_text.hpp"
#include "plt_xml.hpp"

class CL_Font;


class Theme
{
private:
  PLTXML plt;
  CL_Surface surface;
  CL_Surface background;
  CL_Font font;
  CL_Font title;

  /** The last level, which is accessible */
  int      accessible_levels;

  /** The currently selected level, valid in the interval [0,
      accessible_levels */
  int      current_level;
  std::string filename;

  /** The name of the levels, this is was is displayed */
  std::vector<std::string> levelnames;

  /** The filenames of the levels */
  std::vector<std::string> level_filenames;

  std::string title_name;
  std::string status_file;

  /** This is the position at which the levelnames are printed, it is
      used to calculate mark_level_at_point() */
  int level_start_y_pos;

  bool has_description;
  MultiLineText description;

  bool is_loaded;

  void load_status(std::string);
  void load_levels();

public:
  Theme (const std::string& filename);
  ~Theme ();

  void preload ();

  /** Marks the level, which is under the given coordinates
      @param x X-Position (normaly CL_Mouse::get_x())
      @param y Y-Position (normaly CL_Mouse::get_y()) */
  int mark_level_at_point (int x, int y);
  void next_level ();
  void previous_level ();
  void load (const std::string& filename);
  void draw_title ();
  void play ();

private:
  Theme (const Theme&);
  Theme& operator= (const Theme&);
};


#endif

/* EOF */
