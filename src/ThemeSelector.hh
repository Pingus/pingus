//  $Id: ThemeSelector.hh,v 1.7 2000/06/18 17:01:50 grumbel Exp $
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
}///
;

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
}///
;

///
class ThemeSelector
{
public:
  ///
  class Event : public CL_Event_ButtonPress, public CL_Event_ButtonRelease
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

  ///
  CL_Font* title_font;
  ///
  CL_Font* theme_font;
  ///
  CL_Surface* right_arrow;
  ///
  CL_Surface* left_arrow;

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
}///
;

#endif

/* EOF */
