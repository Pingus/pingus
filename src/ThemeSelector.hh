//  $Id: ThemeSelector.hh,v 1.10 2000/06/21 20:29:21 grumbel Exp $
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

///
class ListItem
{
private:
  ///
  std::string label;
  ///
  CL_Font* font;
public:
  ///
  ListItem(std::string);
  ///
  void draw_offset(int x, int y);
  ///
  bool mouse_over();
};

///
class ListBox
{
private:
  ///
  std::vector<ListItem> items;
  ///
  typedef std::vector<ListItem>::iterator ListIter;
public:
  ///
  ListBox();
  ///
  void add_item(std::string);
  ///
  void draw_offset(int x, int y);
};

///
class ThemeSelector
{
public:
  ///
  class Event : public CL_Event_ButtonPress, 
		public CL_Event_ButtonRelease,
		public CL_Event_MouseMove
  {
  public:
    ///
    bool enabled;
    ///
    ThemeSelector* theme_selector;
    ///
    virtual bool on_button_press(CL_InputDevice *device, const CL_Key &key);
    ///
    virtual bool on_button_release(CL_InputDevice *device, const CL_Key &key);
    ///
    virtual bool on_mouse_move(CL_InputDevice *device);
  }///
;
  ///
  friend class Event;
  ///
  Event* event;
  
private:
  ///
  std::vector<AlphaButton*> theme;
  ///
  std::vector<Theme*> themes;
  ///
  std::vector<Theme*>::iterator current_theme;

  /** A large font, used for the Title Name */
  CL_Font* title_font;

  /** This font is used for the levelnames */
  CL_Font* theme_font;

  /// The red arrow to the right
  CL_Surface* right_arrow;
  /// The red arrow to the left
  CL_Surface* left_arrow;
  /// The back button
  CL_Surface* back;

  ///
  bool dir_read;
  ///
  bool finished;
  ///
  ListBox list_box;

public:
  ///
  ThemeSelector();
  ///
  ~ThemeSelector();
 
  ///
  void   readdir(std::string path);
  ///
  void   select();
  ///
  void   draw();
  ///
  bool key_pressed(int key);

  /** Marks the level, which is under the given coordinates 
      @param x X-Position (normaly CL_Mouse::get_x())
      @param y Y-Position (normaly CL_Mouse::get_y())
      @return The current_level at point or -1 if no level is under point */
  int mark_level_at_point(int x, int y);
};

#endif

/* EOF */
