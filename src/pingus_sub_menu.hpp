//  $Id: pingus_sub_menu.hpp 3626 2008-07-02 20:13:11Z grumbel $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_PINGUS_PINGUS_SUB_MENU_HPP
#define HEADER_PINGUS_PINGUS_SUB_MENU_HPP

#include "screen/gui_screen.hpp"


class PingusMenuManager;

class PingusSubMenu : public GUIScreen
{
protected:
  PingusMenuManager* manager;

public:
  PingusSubMenu (PingusMenuManager* m);
  virtual ~PingusSubMenu ();

  /// Return a handle to the parent menu manager
  PingusMenuManager* get_manager () { return manager; }

private:
  PingusSubMenu (const PingusSubMenu&);
  PingusSubMenu& operator= (const PingusSubMenu&);
};


#endif

/* EOF */
