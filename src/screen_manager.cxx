//  $Id: screen_manager.cxx,v 1.3 2002/08/01 21:40:01 grumbel Exp $
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
#include "input/controller.hxx"
#include "delta_manager.hxx"
#include "game_delta.hxx"
#include "screen.hxx"
#include "display.hxx"
#include "pingus_menu_manager.hxx"
#include "screen_manager.hxx"

ScreenManager* ScreenManager::instance_ = 0;

ScreenManager::ScreenManager ()
{
  push_screen (PingusMenuManager::instance (), false);
}

ScreenManager::~ScreenManager ()
{
}

void
ScreenManager::display ()
{
  Input::Controller input_controller("../doc/mycontroller.xml");
  DeltaManager delta_manager;

  // Main loop for the menu
  while (!screens.empty())
    {
      Screen* current_screen = screens.back ().first;
      float time_delta = delta_manager.getset ();

      if (time_delta > 1.0)
	{
	  std::cout << "PingusMenuManager: detected large delta (" << time_delta
		    << "), ignoring and doing frameskip" << std::endl;
	  continue;
	}

      GameDelta delta (time_delta, input_controller.get_events ());

      input_controller.update (time_delta);

      CL_System::keep_alive ();
      current_screen->draw ();
      current_screen->update (delta);

      Display::flip_display ();
      // Stupid hack to make this thing take less CPU
      CL_System::sleep (0);
    } 
}

ScreenManager*
ScreenManager::instance ()
{
  if (instance_)
    return instance_;
  else
    return instance_ = new ScreenManager ();
}

void
ScreenManager::push_screen (Screen* screen, bool delete_screen)
{
  screens.push_back (std::pair<Screen*, bool> (screen, delete_screen));
}

void
ScreenManager::pop_screen ()
{
  Screen* current_screen = screens.back ().first;
  bool    delete_screen  = screens.back ().second;

  if (delete_screen)
    delete current_screen;

  screens.pop_back ();
}

void
ScreenManager::replace_screen (Screen* screen, bool delete_screen)
{
  Screen* current_screen = screens.back ().first;
  bool    delete_c_screen  = screens.back ().second;

  if (delete_c_screen)
    delete current_screen;

  screens.back () = std::pair<Screen*, bool>(screen, delete_screen);
}

/* EOF */
