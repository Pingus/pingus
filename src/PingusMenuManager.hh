//  $Id: PingusMenuManager.hh,v 1.2 2001/06/14 14:45:23 grumbel Exp $
// 
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef PINGUSMENUMANAGER_HH
#define PINGUSMENUMANAGER_HH

#include <list>
#include "boost/dummy_ptr.hpp"
#include "PingusSubMenu.hh"
#include "MenuBackground.hh"
#include "ExitMenu.hh"
#include "Story.hh"

#include "PingusMenu.hh"
#include "Intro.hh"

class PingusMenuManager
{
private:
  /** Menu stack. the menu on top is .end (), the bottom menu .begin () */
  std::list<boost::dummy_ptr<PingusSubMenu> > menu_stack;
  typedef std::list<boost::dummy_ptr<PingusSubMenu> >::iterator MenuStackIter;
  typedef std::list<boost::dummy_ptr<PingusSubMenu> >::reverse_iterator MenuStackRIter;

  bool loop;

  CL_Slot on_mouse_move_slot;
  CL_Slot on_button_press_slot;
  CL_Slot on_button_release_slot;
  int event_register_counter;
  
public:
  /* Menu's */
  Intro intro;
  PingusMenu mainmenu;
  OptionMenu optionmenu;
  MenuBackground background;
  Story story;
  ExitMenu exitmenu;

  PingusMenuManager ();
  ~PingusMenuManager ();

  void draw ();
  void update (float delta);

  /// Display the menu (this is basically the game's main loop)
  void display ();

  /// Register all event-handling stuff
  void register_events ();

  /// Unregister all event-handling stuff
  void unregister_events ();

  void fadeout ();

  // !FIXME! Should use controller instead
  void on_button_press (CL_InputDevice*,const CL_Key &);
  void on_button_release (CL_InputDevice*,const CL_Key &);
  void on_mouse_move (CL_InputDevice*, int, int);
  
  /// Exit the menu manager (which means to exit the while() loop in display ())
  void exit ();

  boost::dummy_ptr<PingusSubMenu> current_menu ();

  // Replace the current menu with a new one
  void set_menu (boost::dummy_ptr<PingusSubMenu> menu);

  /** Remove the current menu and fall back to the last one.
      This is used for yes/no dialoges and such things. */
  void pop_menu ();
  
  /** Add's a new menu to the menu stack. This is used for yes/no
      dialoges and such things.*/
  void push_menu (boost::dummy_ptr<PingusSubMenu> menu);
};

#endif

/* EOF */
