//  $Id: global_event.cxx,v 1.15 2003/10/22 11:11:22 grumbel Exp $
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

#include <config.h>
#include <stdio.h>
#include <algorithm>
#include "screenshot.hpp"
#include "console.hpp"
#include "fps_counter.hpp"
#include "global_event.hpp"
#include "globals.hpp"

GlobalEvent global_event;

GlobalEvent::GlobalEvent ()
{
}

void
GlobalEvent::on_button_press(const SDL_KeyboardEvent& event)
{
  Uint8 *keystate = SDL_GetKeyState(NULL);

  std::cout << "GlobalEvent: " << event.keysym.sym << std::endl;
  switch (event.keysym.sym)
    {
    case SDLK_F1:
      console.toggle_display();
      break;

    case SDLK_F10:
      fps_counter.toggle_display();
      console << "Toggling fps counter display" << std::endl;
      break;

    case SDLK_F11:
      fullscreen_enabled = !fullscreen_enabled;
      // re-initialize the screen
      Display::set_video_mode(screen_width, screen_height);
      break;


    case SDLK_F12:
      {
        std::string filename;
        std::cout << "GlobalEvent::Making screenshot..." << std::endl;
        filename = Screenshot::make_screenshot();
        console << "GlobalEvent: Saved screenshot to \"" << filename << "\"" << std::endl;
        //console << "!\"#$%&'()*+,-./0123456789:;<=>?@";
        console.newline();
      }
      break;

    case SDLK_c:
      if (maintainer_mode)
        draw_collision_map = !draw_collision_map;
      break;

    case SDLK_m:
      if (keystate[SDLK_LCTRL] || keystate[SDLK_RCTRL])
        {
          std::cout << "Maintainer Mode: " << maintainer_mode << std::endl;
          maintainer_mode = !maintainer_mode;
        }
      break;

    default:
      // console << "GlobalEvent: Unknown key pressed: " << key.id;
      break;
    }
}

void
GlobalEvent::on_button_release(const SDL_KeyboardEvent& event)
{
}


/* EOF */
