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

#ifndef HEADER_PINGUS_EXIT_MENU_HPP
#define HEADER_PINGUS_EXIT_MENU_HPP

#include "pingus_sub_menu.hpp"


class PingusMenuManager;

class ExitMenu : public PingusSubMenu
{
private:
  Sprite sur;

public:
  ExitMenu (PingusMenuManager* manager);
  ~ExitMenu ();

  bool draw (DrawingContext& gc);

private:
  ExitMenu (const ExitMenu&);
  ExitMenu& operator= (const ExitMenu&);
};


#endif

/* EOF */
