// Pingus - A free Lemmings clone
// Copyright (C) 2000 Ingo Ruhnke <grumbel@gmail.com>
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

#include "pingus/global_event.hpp"

#include <logmich/log.hpp>

#include "engine/display/screenshot.hpp"
#include "engine/screen/screen_manager.hpp"
#include "pingus/config_manager.hpp"
#include "pingus/globals.hpp"
#include "pingus/screens/addon_menu.hpp"
#include "pingus/screens/option_menu.hpp"
#include "util/system.hpp"

namespace pingus {

GlobalEvent global_event;

GlobalEvent::GlobalEvent ()
{
}

void
GlobalEvent::on_button_press(SDL_KeyboardEvent const& event)
{
  switch (event.keysym.sym)
  {
    case SDLK_F10:
      config_manager.set_print_fps(!config_manager.get_print_fps());
      break;

    case SDLK_RETURN:
      if (event.keysym.mod & KMOD_ALT)
      {
        config_manager.set_fullscreen(!config_manager.get_fullscreen());
      }
      break;

    case SDLK_TAB: // unlock mouse grab if Alt-Tab is pressed to allow the user to change windows
      if (config_manager.get_mouse_grab())
      {
        if (event.keysym.mod & KMOD_ALT)
        {
          // FIXME: should suspend the grab till the user clicks the
          // window again, not completely disable it
          config_manager.set_mouse_grab(false);
        }
      }
      break;

    case SDLK_F11:
      config_manager.set_fullscreen(!config_manager.get_fullscreen());
      break;

    case SDLK_F5:
      if (!dynamic_cast<OptionMenu*>(ScreenManager::instance()->get_current_screen().get()))
        ScreenManager::instance()->push_screen(std::make_shared<OptionMenu>());
      break;

    case SDLK_o:
      if (event.keysym.mod & KMOD_CTRL)
      {
        if (!dynamic_cast<OptionMenu*>(ScreenManager::instance()->get_current_screen().get()))
          ScreenManager::instance()->push_screen(std::make_shared<OptionMenu>());
      }
      break;

    case SDLK_F6:
      if (globals::developer_mode)
      {
        if (!dynamic_cast<AddOnMenu*>(ScreenManager::instance()->get_current_screen().get()))
          ScreenManager::instance()->push_screen(std::make_shared<AddOnMenu>());
      }
      break;

    case SDLK_F12:
      {
        auto get_date = []() -> std::string {
          char buffer[64];
          time_t curtime;
          struct tm *loctime;
          curtime = time (nullptr);
          loctime = localtime(&curtime);
          strftime(buffer, 64, "%Y%m%d-%H%M%S", loctime);

          return std::string(buffer);
        };

        auto get_filename = [get_date]() -> std::string {
          std::string tmp_filename;
          char str [16];
          int i = 1;

          do {
            snprintf(str, 16, "%d.png", i);
            tmp_filename = System::get_userdir() + "screenshots/"
              + "pingus-" + get_date() + "-" + std::string(str);
            ++i;
          } while (std::filesystem::exists(tmp_filename));

          return tmp_filename;
        };

        Screenshot::save_screenshot(get_filename());
      }
      break;

    case SDLK_c:
      if (globals::developer_mode)
        globals::draw_collision_map = !globals::draw_collision_map;
      break;

    case SDLK_k:
      if (globals::developer_mode)
      {
#ifdef OLD_SDL1
        log_info("Low level screen clear triggered");
        SDL_Surface* screen = SDL_GetVideoSurface();
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 0));
        SDL_Flip(screen);
#endif
      }
      break;

    case SDLK_m:
      if (event.keysym.mod & KMOD_CTRL)
      {
        log_info("Developer Mode: {}", globals::developer_mode);
        globals::developer_mode = !globals::developer_mode;
      }
      break;

    case SDLK_g:
      if (event.keysym.mod & KMOD_CTRL)
      {
        config_manager.set_mouse_grab(!config_manager.get_mouse_grab());
      }
      break;

    case SDLK_KP_PLUS:
      globals::game_speed -= 5;
      if (globals::game_speed < 5)
        globals::game_speed = 5;
      break;

    case SDLK_KP_MINUS:
      globals::game_speed += 5;
      break;

    case SDLK_KP_ENTER:
      globals::game_speed = 20;
      break;

    default:
      // console << "GlobalEvent: Unknown key pressed: " << key.id;
      break;
  }
}

void
GlobalEvent::on_button_release(SDL_KeyboardEvent const& event)
{
}

} // namespace pingus

/* EOF */
