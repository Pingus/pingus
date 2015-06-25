// Pingus - A free Lemmings clone
// Copyright (C) 1998-2009 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <iostream>
#include <stdexcept>
#include <SDL.h>

#include "util/pathname.hpp"
#include "engine/input/manager.hpp"
#include "engine/input/controller.hpp"
#include "engine/system/sdl_system.hpp"

int main()
{
  try
  {
    SDLSystem sdl_system;

    Input::Manager manager;

    Input::ControllerPtr controller
      = manager.create_controller(Pathname("../data/controller/default.scm", Pathname::SYSTEM_PATH));

    while(true)
    {
      std::vector<Input::Event> events;

      controller->poll_events(events);

      for(std::vector<Input::Event>::iterator i = events.begin(); i != events.end(); ++i)
      {
        // insert code here
      }

      manager.update(0.033f);
    }
  }
  catch (std::exception& err)
  {
    std::cout << "Exception: " << err.what() << std::endl;
  }

  return 0;
}

/* EOF */
