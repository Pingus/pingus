//  $Id: pingus_menu_manager.hxx,v 1.22 2003/10/18 23:17:27 grumbel Exp $
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

#ifndef HEADER_PINGUS_PINGUS_MENU_MANAGER_HXX
#define HEADER_PINGUS_PINGUS_MENU_MANAGER_HXX

#include "menu_background.hxx"
#include "exit_menu.hxx"
#include "pingus_menu.hxx"
#include "sprite.hxx"

namespace Pingus {

class PingusSubMenu;

class PingusMenuManager : public Screen
{
private:
  static PingusMenuManager* instance_;
  /** Menu stack. the menu on top is .end (), the bottom menu .begin () */
  std::vector<PingusSubMenu *> menu_stack;
  typedef std::vector<PingusSubMenu *>::iterator MenuStackIter;
  typedef std::vector<PingusSubMenu *>::reverse_iterator MenuStackRIter;

  /// Register all event-handling stuff
  void register_events ();

  /// Unregister all event-handling stuff
  void unregister_events ();
public:
  /* Menu's FIXME: These shouldn't get exported to the outsite,
     instead only handles (enum's) should be visible */
  PingusMenu mainmenu;
  MenuBackground background;
  ExitMenu exitmenu;

  virtual ~PingusMenuManager ();

  void fadeout ();

  bool draw (GraphicContext& gc);
  void update (const GameDelta&);

  // !FIXME! Should use controller instead
  /*
  void on_button_press (CL_InputDevice*,const CL_Key &);
  void on_button_release (CL_InputDevice*,const CL_Key &);
  void on_mouse_move (CL_InputDevice*, int, int);
  */

  /// Exit the menu manager (which means to exit the while() loop in display ())
  void show_exit_menu ();

  void exit ();

  PingusSubMenu * current_menu();

  // Replace the current menu with a new one
  void set_menu (PingusSubMenu * menu);
  /** Remove the current menu and fall back to the last one.
      This is used for yes/no dialoges and such things. */
  void pop_menu ();

  /** Add's a new menu to the menu stack. This is used for yes/no
      dialoges and such things.*/
  void push_menu (PingusSubMenu * menu);

  void on_startup();

  static PingusMenuManager* instance ();
  static void init();
  static void deinit();

protected:
  PingusMenuManager ();

private:
  PingusMenuManager (const PingusMenuManager&);
  PingusMenuManager& operator= (const PingusMenuManager&);
};

} // namespace Pingus

#endif

/* EOF */
