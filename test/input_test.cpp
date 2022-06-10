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
#include "engine/input/driver_factory.hpp"
#include "engine/system/sdl_system.hpp"
#include "pingus/event_name.hpp"

using namespace pingus;

int main()
{
  try
  {
    SDLSystem sdl_system;

    pingus::input::ControllerDescription desc;
    desc.add_axis("action-axis",  ACTION_AXIS);

    desc.add_keyboard("standard-keyboard",  STANDARD_KEYBOARD);

    desc.add_pointer("standard-pointer",   STANDARD_POINTER);
    desc.add_scroller("standard-scroller", STANDARD_SCROLLER);

    desc.add_button("primary-button",      PRIMARY_BUTTON);
    desc.add_button("secondary-button",    SECONDARY_BUTTON);
    desc.add_button("fast-forward-button", FAST_FORWARD_BUTTON);
    desc.add_button("armageddon-button",   ARMAGEDDON_BUTTON);
    desc.add_button("pause-button",        PAUSE_BUTTON);
    desc.add_button("single-step-button",  SINGLE_STEP_BUTTON);
    desc.add_button("escape-button",       ESCAPE_BUTTON);

    desc.add_button("action-up-button",    ACTION_UP_BUTTON);
    desc.add_button("action-down-button",  ACTION_DOWN_BUTTON);

    desc.add_button("action-1-button",     ACTION_1_BUTTON);
    desc.add_button("action-2-button",     ACTION_2_BUTTON);
    desc.add_button("action-3-button",     ACTION_3_BUTTON);
    desc.add_button("action-4-button",     ACTION_4_BUTTON);
    desc.add_button("action-5-button",     ACTION_5_BUTTON);
    desc.add_button("action-6-button",     ACTION_6_BUTTON);
    desc.add_button("action-7-button",     ACTION_7_BUTTON);
    desc.add_button("action-8-button",     ACTION_8_BUTTON);
    desc.add_button("action-9-button",     ACTION_9_BUTTON);
    desc.add_button("action-10-button",    ACTION_10_BUTTON);

    pingus::input::SDLDriverFactory driver_factory;
    pingus::input::Manager manager(driver_factory, desc);

    pingus::input::ControllerPtr controller
      = manager.create_controller(Pathname("../data/controller/default.scm", Pathname::SYSTEM_PATH).get_sys_path());

    while(true)
    {
      std::vector<pingus::input::Event> events;

      controller->poll_events(events);

      for(std::vector<pingus::input::Event>::iterator i = events.begin(); i != events.end(); ++i)
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
