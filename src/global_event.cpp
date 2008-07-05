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

#include <config.h>
#include <stdio.h>
#include <algorithm>
#include "screenshot.hpp"
#include "console.hpp"
#include "fps_counter.hpp"
#include "screen/screen_manager.hpp"
#include "option_menu.hpp"
#include "level_menu.hpp"
#include "addon_menu.hpp"
#include "global_event.hpp"
#include "config_manager.hpp"
#include "globals.hpp"

GlobalEvent global_event;

GlobalEvent::GlobalEvent ()
{
}

void
GlobalEvent::on_button_press(const SDL_KeyboardEvent& event)
{
  Uint8 *keystate = SDL_GetKeyState(NULL);

  switch (event.keysym.sym)
    {
      case SDLK_F1:
        if (console.is_visible())
          console.hide();
        else
          console.show();
        break;

      case SDLK_F10:
        config_manager.set_print_fps(!config_manager.get_print_fps());
        break;

      case SDLK_RETURN:
        if (!keystate[SDLK_LALT] && !keystate[SDLK_RALT])
          break;
        // FALL THROUGH
      case SDLK_F11:
        config_manager.set_fullscreen(!config_manager.get_fullscreen());
        break;

      case SDLK_F5:
        if (maintainer_mode)
          if (!dynamic_cast<OptionMenu*>(ScreenManager::instance()->get_screen()))
            ScreenManager::instance()->push_screen(new OptionMenu());
        break;

      case SDLK_F6:
        if (maintainer_mode)
          if (!dynamic_cast<AddOnMenu*>(ScreenManager::instance()->get_screen()))
            ScreenManager::instance()->push_screen(new AddOnMenu());
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

      case SDLK_g:
        if (keystate[SDLK_LCTRL] || keystate[SDLK_RCTRL])
          {
            config_manager.set_mouse_grab(!config_manager.get_mouse_grab());
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
