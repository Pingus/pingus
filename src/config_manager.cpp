//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
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

#include <iostream>
#include "SDL.h"
#include "globals.hpp"
#include "fps_counter.hpp"
#include "display/display.hpp"
#include "screen/screen_manager.hpp"
#include "config_manager.hpp"

ConfigManager config_manager;

ConfigManager::ConfigManager()
{
}

ConfigManager::~ConfigManager()
{
}

void
ConfigManager::set_master_volume(int v)
{
  if (maintainer_mode)
    std::cout << "ConfigManager::set_master_volume: " << v << std::endl;
}

int
ConfigManager::get_master_volume()
{
  return 0;
}

void
ConfigManager::set_sound_volume(int v)
{
  if (maintainer_mode)
    std::cout << "ConfigManager::set_sound_volume: " << v << std::endl;
}

int
ConfigManager::get_sound_volume()
{
  return 0;
}

void
ConfigManager::set_music_volume(int v)
{
  if (maintainer_mode)
    std::cout << "ConfigManager::set_music_volume: " << v << std::endl;
}

int
ConfigManager::get_music_volume()
{
  return 0;
}

void
ConfigManager::set_resolution(const Size& size)
{
  if (maintainer_mode)
    std::cout << "ConfigManager::set_resolution: " << size.width << "x" << size.height << std::endl;

  if (size != get_resolution())
    {
      screen_width  = size.width;
      screen_height = size.height;
      Display::set_video_mode(screen_width, screen_height, fullscreen_enabled);
      on_resolution_change(size);
    }
}

Size
ConfigManager::get_resolution()
{
  return Display::get_size();
}

void
ConfigManager::set_fullscreen(bool v)
{
  if (maintainer_mode)
    std::cout << "ConfigManager::set_fullscreen: " << v << std::endl;

  if (v != get_fullscreen())
    {
      fullscreen_enabled = v;
      Size screen_size = Display::get_size();
      Display::set_video_mode(screen_size.width, screen_size.height, fullscreen_enabled);
      on_fullscreen_change(v);
    }
}

bool
ConfigManager::get_fullscreen()
{
  return fullscreen_enabled;
}

void
ConfigManager::set_allow_resize(bool v)
{
  if (maintainer_mode)
    std::cout << "ConfigManager::set_allow_resize: " << v << std::endl;
}

bool
ConfigManager::get_allow_resize()
{
  return false;
}

void
ConfigManager::set_mouse_grab(bool v)
{
  if (maintainer_mode)
    std::cout << "ConfigManager::set_mouse_grab: " << v << std::endl;

  if (v != get_mouse_grab())
    {
      if (v)
        SDL_WM_GrabInput(SDL_GRAB_ON);
      else
        SDL_WM_GrabInput(SDL_GRAB_OFF);

      on_mouse_grab_change(v);
    }
}

bool
ConfigManager::get_mouse_grab()
{
  return (SDL_WM_GrabInput(SDL_GRAB_QUERY) == SDL_GRAB_ON);
}

void
ConfigManager::set_print_fps(bool v)
{
  if (maintainer_mode)
    std::cout << "ConfigManager::set_print_fps: " << v << std::endl;

  print_fps = v;
  on_print_fps_change(v);
}

bool
ConfigManager::get_print_fps()
{
  return print_fps;
}

void
ConfigManager::set_language(const std::string& v)
{
  if (maintainer_mode)
    std::cout << "ConfigManager::set_language: '" << v << "'" << std::endl;
}

std::string
ConfigManager::get_language()
{
  return "";
}

void
ConfigManager::set_swcursor(bool v)
{
  if (maintainer_mode)
    std::cout << "ConfigManager::set_swcursor: " << v << std::endl;

  if (v != get_swcursor())
    {
      ScreenManager::instance()->show_swcursor(v);
    }
}

bool
ConfigManager::get_swcursor()
{
  return swcursor_enabled;
}

void
ConfigManager::set_autoscroll(bool v)
{
  if (maintainer_mode)
    std::cout << "ConfigManager::set_autoscroll: " << v << std::endl;
}

bool
ConfigManager::get_autoscroll()
{
  return false;
}

/* EOF */
