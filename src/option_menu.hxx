//  $Id: option_menu.hxx,v 1.6 2002/09/27 11:26:43 torangan Exp $
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

#ifndef HEADER_PINGUS_OPTION_MENU_HXX
#define HEADER_PINGUS_OPTION_MENU_HXX

#include <string>
#include <vector>
#include <ClanLib/Display/Display/surface.h>
#include "pingus_sub_menu.hxx"

class CL_Key;
class CL_Font;
class CL_InputDevice;

class OptionEntry
{
private:
  bool*        value_bool;
  int*         value_int;
  std::string* value_str;
  CL_Font*     font;
  int          x_pos;
  int          y_pos;
  std::string  str;

public:
  OptionEntry (const std::string&, bool*, int, int);
  OptionEntry (const std::string&, int*, int, int);
  OptionEntry (const std::string&, std::string*, int, int);
  
  OptionEntry (const OptionEntry& old);
  OptionEntry& operator= (const OptionEntry& old);

  ~OptionEntry ();
    
  bool mouse_over();
  void draw(GraphicContext& gc);
  void toggle();
  void rtoggle();
};

/** F***ing, stupid, ugly option menu, need to rewrite that... */
class OptionMenu : public PingusSubMenu
{
private:
  CL_Font* font;
  CL_Font* title_font;
  CL_Surface background;
  CL_Surface back;
  bool quit;
  bool is_init;
  int entry_x, entry_y;
  typedef std::vector<OptionEntry>::iterator EntryIter;
  std::vector<OptionEntry> entry;

  class Event //: public CL_Event_ButtonPress, public CL_Event_ButtonRelease
  {
  public:
    OptionMenu* option_menu;
    
  public:
    Event ();
    
    virtual void on_button_press (CL_InputDevice *device, const CL_Key &key);
    virtual void on_button_release (CL_InputDevice *device, const CL_Key &key);
    
  private:
    Event (const Event&);
    Event& operator= (const Event&);
  };
  
  
  friend class Event;
  Event* event;

public:
  OptionMenu (PingusMenuManager* m);
  ~OptionMenu ();

  void init ();
  void display ();
  void draw_background ();
  void preload () { init (); }
  void update (float delta);
  void draw (GraphicContext& gc);
  void check_click ();
  EntryIter current_item ();
  void add_entry (const std::string& e, bool* v);
  void add_entry (const std::string& e, int* v);
  void add_entry (const std::string& e, std::string* v);
  
private:
  OptionMenu (const OptionMenu&);
  OptionMenu& operator= (const OptionMenu&);
};

extern OptionMenu option_menu;

#endif

/* EOF */
