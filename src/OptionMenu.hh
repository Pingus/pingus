//  $Id: OptionMenu.hh,v 1.13 2002/06/10 11:00:26 torangan Exp $
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
#include "PingusSubMenu.hh"

class CL_Key;
class CL_InputDevice;

///
class OptionEntry
{
private:
  ///
  std::string str;
  ///
  bool*   value_bool;
  ///
  std::string* value_str;
  ///
  int*    value_int;
  ///
  int x_pos, y_pos;
  ///
  CL_Font* font;

public:
  ///
  OptionEntry(std::string, bool*, int, int);
  ///
  OptionEntry(std::string, int*, int, int);
  ///
  OptionEntry(std::string, std::string*, int, int);
  ///
  bool mouse_over();
  ///
  void draw();
  ///
  void toggle();
  ///
  void rtoggle();
}///
;

/** F***ing, stupid, ugly option menu, need to rewrite that... */
class OptionMenu : public PingusSubMenu
{
private:
  ///
  CL_Font* font;
  ///
  CL_Font* title_font;
  ///
  CL_Surface background;
  ///
  CL_Surface back;
  ///
  bool quit;
  ///
  bool is_init;
  ///
  int entry_x, entry_y;
  ///
  typedef std::vector<OptionEntry>::iterator EntryIter;
  ///
  std::vector<OptionEntry> entry;

  ///
  class Event //: public CL_Event_ButtonPress, public CL_Event_ButtonRelease
  {
  public:
    ///
    OptionMenu* option_menu;
    ///
    virtual void on_button_press(CL_InputDevice *device, const CL_Key &key);
    ///
    virtual void on_button_release(CL_InputDevice *device, const CL_Key &key);
  };
  ///
  friend class Event;
  ///
  Event* event;

public:
  ///
  OptionMenu(PingusMenuManager* m);
  ///
  ~OptionMenu();

  ///
  void init();
  ///
  void display();
  ///
  void draw_background();
  ///
  void preload () { init (); }
  void update (float delta);
  void draw();
  ///
  void check_click();
  ///
  EntryIter current_item();
  ///
  void add_entry(std::string e, bool* v);
  ///
  void add_entry(std::string e, int* v);
  ///
  void add_entry(std::string e, std::string* v);
};

///
extern OptionMenu option_menu;

#endif

/* EOF */

