//  $Id: exit_menu.cxx,v 1.11 2002/10/01 21:48:32 grumbel Exp $
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

#include <iostream>
#include <ClanLib/Display/Display/display.h>
#include "pingus_menu_manager.hxx"
#include "pingus_resource.hxx"
#include "gui/gui_manager.hxx"

class ExitMenuYesButton : public GUI::Component
{
private:
  PingusMenuManager* manager;
public:
  ExitMenuYesButton (PingusMenuManager* m) 
    : manager (m)
  {
  }
 
  bool is_at (int x, int y) 
  {
    return x > 270 && x < 390 && y > 300 && y < 380;
  }

  void draw (GraphicContext& gc) { UNUSED_ARG(gc); }

  void on_primary_button_press (int, int)
  {
    std::cout << "YES" << std::endl; 
    manager->exit ();
  }
};

class ExitMenuNoButton : public GUI::Component
{
private:
  PingusMenuManager* manager;
public:
  ExitMenuNoButton (PingusMenuManager* m)
    : manager (m)
  {
  }
  
  bool is_at (int x, int y) 
  {
    return x > 430 && x < 540 && y > 300 && y < 380;
  }

  void draw (GraphicContext& gc) { UNUSED_ARG(gc); }

  void on_primary_button_press (int, int)
  {
    std::cout << "NO" << std::endl; 
    manager->pop_menu ();
  }
};

ExitMenu::ExitMenu (PingusMenuManager* manager)
  : PingusSubMenu (manager)
{
  gui_manager->add (new ExitMenuYesButton (manager), true);
  gui_manager->add (new ExitMenuNoButton (manager), true);
}

ExitMenu::~ExitMenu ()
{
}

bool 
ExitMenu::draw (GraphicContext& gc)
{
  CL_Display::fill_rect (0, 0, CL_Display::get_width (), CL_Display::get_height (),
			 0, 0, 0, 0.5);
  sur.put_screen (CL_Display::get_width ()/2 - sur.get_width ()/2, 
		  CL_Display::get_height ()/2 - sur.get_height ()/2);
  UNUSED_ARG(gc);
  return true;
}

void 
ExitMenu::update (float /*delta*/)
{
}

void 
ExitMenu::preload ()
{
  sur = PingusResource::load_surface ("misc/exitmenu", "core");
}

/* EOF */
