//  $Id: OptionMenu.hh,v 1.2 2000/02/09 21:43:40 grumbel Exp $
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

#ifndef OPTIONMENU_HH
#define OPTIONMENU_HH

#include <vector>

#include <ClanLib/core.h>


class OptionEntry
{
private:
  string str;
  bool*   value_bool;
  string* value_str;
  int*    value_int;
  int x_pos, y_pos;
  CL_Font* font;

public:
  OptionEntry(string, bool*, int, int);
  OptionEntry(string, int*, int, int);
  OptionEntry(string, string*, int, int);
  bool mouse_over();
  void draw();
  void toggle();
  void rtoggle();
};

class OptionMenu
{
private:
  CL_Font* font;
  CL_Font* title_font;
  CL_Surface* background;
  CL_Surface* cursor_sur;
  bool quit;
  bool is_init;
  int entry_x, entry_y;
  typedef vector<OptionEntry>::iterator EntryIter;
  vector<OptionEntry> entry;

  class Event : public CL_Event_ButtonPress, public CL_Event_ButtonRelease
  {
  public:
    OptionMenu* option_menu;
    virtual bool on_button_press(CL_InputDevice *device, const CL_Key &key);
    virtual bool on_button_release(CL_InputDevice *device, const CL_Key &key);
  };
  friend class Event;
  Event* event;

public:
  OptionMenu();
  ~OptionMenu();

  void init();
  void display();
  void draw_background();
  void draw();
  void check_click();
  EntryIter current_item();
  void add_entry(string e, bool* v);
  void add_entry(string e, int* v);
  void add_entry(string e, string* v);
};

extern OptionMenu option_menu;

#endif

/* EOF */
