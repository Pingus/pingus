//  $Id: ThemeSelector.hh,v 1.1 2000/02/04 23:45:19 mbn Exp $
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

#ifndef THEMESELECTOR_HH
#define THEMESELECTOR_HH

#include <ClanLib/core.h>


#include <vector>
#include <string>

#include "Theme.hh"
#include "AlphaButton.hh"

class ListItem
{
private:
  string label;
  CL_Font* font;
public:
  ListItem(string);
  void draw_offset(int x, int y);
  bool mouse_over();
};

class ListBox
{
private:
  vector<ListItem> items;
  typedef vector<ListItem>::iterator ListIter;
public:
  ListBox();
  void add_item(string);
  void draw_offset(int x, int y);
};

class ThemeSelector
{
private:
  vector<AlphaButton*> theme;
  vector<Theme*> themes;
  vector<Theme*>::iterator current_theme;
  CL_Font* title_font;
  CL_Font* theme_font;
  bool dir_read;
  ListBox list_box;
public:
  ThemeSelector();
  ~ThemeSelector();
 
  void   readdir(string path);
  void   select();
  void   draw();
  bool key_pressed(int key);
};

#endif

/* EOF */
