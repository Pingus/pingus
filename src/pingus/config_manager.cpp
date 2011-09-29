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

#include "pingus/config_manager.hpp"

#include "engine/display/display.hpp"
#include "engine/screen/screen_manager.hpp"
#include "lisp/parser.hpp"
#include "pingus/globals.hpp"
#include "tinygettext/dictionary_manager.hpp"
#include "util/log.hpp"
#include "util/sexpr_file_reader.hpp"
#include "util/sexpr_file_writer.hpp"
#include "util/system.hpp"
#include "engine/sound/sound.hpp"

extern tinygettext::DictionaryManager dictionary_manager;

ConfigManager config_manager;

ConfigManager::ConfigManager() :
  on_master_volume_change(),
  on_sound_volume_change(),
  on_music_volume_change(),
  on_fullscreen_resolution_change(),
  on_fullscreen_change(),
  on_allow_resize_change(),
  on_mouse_grab_change(),
  on_print_fps_change(),
  on_language_change(),
  on_software_cursor_change(),
  on_autoscroll_change(),
  filename()
{
}

ConfigManager::~ConfigManager()
{
}

void
ConfigManager::set_master_volume(int v)
{
  log_info("ConfigManager::set_master_volume: " << v);
  Sound::PingusSound::set_master_volume(static_cast<float>(v) / 100.0f);
}

int
ConfigManager::get_master_volume()
{
  return static_cast<int>(Sound::PingusSound::get_master_volume() * 100);
}

void
ConfigManager::set_sound_volume(int v)
{
  log_info("ConfigManager::set_sound_volume: " << v);
  Sound::PingusSound::set_sound_volume(static_cast<float>(v) / 100.0f);
}

int
ConfigManager::get_sound_volume()
{
  return static_cast<int>(Sound::PingusSound::get_sound_volume() * 100);
}

void
ConfigManager::set_music_volume(int v)
{
  log_info("ConfigManager::set_music_volume: " << v);
  Sound::PingusSound::set_music_volume(static_cast<float>(v) / 100.0f);
}

int
ConfigManager::get_music_volume()
{
  return static_cast<int>(Sound::PingusSound::get_music_volume() * 100);
}

void
ConfigManager::set_fullscreen_resolution(const Size& size)
{
  log_info(size.width << "x" << size.height);

  if (size != get_fullscreen_resolution())
  {
    Display::set_video_mode(size, globals::fullscreen_enabled);
    on_fullscreen_resolution_change(size);
  }
}

Size
ConfigManager::get_fullscreen_resolution()
{
  return Display::get_size();
}

void
ConfigManager::set_fullscreen(bool v)
{
  log_info("ConfigManager::set_fullscreen: " << v);

  if (v != get_fullscreen())
  {
    globals::fullscreen_enabled = v;
    Size screen_size = Display::get_size();
    Display::set_video_mode(screen_size, globals::fullscreen_enabled);
    on_fullscreen_change(v);
  }
}

bool
ConfigManager::get_fullscreen()
{
  return globals::fullscreen_enabled;
}

void
ConfigManager::set_allow_resize(bool v)
{
  log_info("ConfigManager::set_allow_resize: " << v);

  if (v != get_allow_resize())
  {
    on_allow_resize_change(v);
  }
}

bool
ConfigManager::get_allow_resize()
{
  return false;
}

void
ConfigManager::set_mouse_grab(bool v)
{
  log_info("ConfigManager::set_mouse_grab: " << v);

  if (v != get_mouse_grab())
  {
    SDL_WM_GrabInput(v ? SDL_GRAB_ON : SDL_GRAB_OFF);
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
  log_info("ConfigManager::set_print_fps: " << v);

  if (v != get_print_fps())
  {
    globals::print_fps = v;
    on_print_fps_change(v);
  }
}

bool
ConfigManager::get_print_fps()
{
  return globals::print_fps;
}

void
ConfigManager::set_language(const tinygettext::Language& v)
{
  log_info("ConfigManager::set_language: '" << v << "'");

  if (v != get_language())
  {
    dictionary_manager.set_language(v);
    on_language_change(v);
  }
}

tinygettext::Language
ConfigManager::get_language()
{
  return dictionary_manager.get_language();
}

void
ConfigManager::set_software_cursor(bool v)
{
  log_info("ConfigManager::set_software_cursor: " << v);

  if (v != get_software_cursor())
  {
    ScreenManager::instance()->show_software_cursor(v);
    on_software_cursor_change(v);
  }
}

bool
ConfigManager::get_software_cursor()
{
  return globals::software_cursor;
}

void
ConfigManager::set_autoscroll(bool v)
{
  log_info("ConfigManager::set_autoscroll: " << v);

  if (v != get_autoscroll())
  {
    globals::auto_scrolling = v;
    on_autoscroll_change(v);
  }
}

bool
ConfigManager::get_autoscroll()
{
  return globals::auto_scrolling;
}

/* EOF */
