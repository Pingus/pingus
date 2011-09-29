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
  on_resolution_change(),
  on_fullscreen_change(),
  on_allow_resize_change(),
  on_mouse_grab_change(),
  on_print_fps_change(),
  on_language_change(),
  on_swcursor_change(),
  on_autoscroll_change(),
  filename()
{
}

ConfigManager::~ConfigManager()
{
}

void
ConfigManager::load(const std::string& file)
{
  filename = file;

  std::shared_ptr<lisp::Lisp> sexpr;

  try 
  {
    sexpr = lisp::Parser::parse(filename);
  }
  catch (const std::runtime_error& e) 
  {
    log_error("ConfigManager: " << e.what());
    return;
  }

  if (!sexpr)
  {
    std::cerr << "ConfigManager: Couldn't find config file '" <<
      filename << "'." << std::endl;
    return;
  }

  SExprFileReader reader(sexpr->get_list_elem(0));
  if (reader.get_name() != "pingus-config")
  {
    log_error("Error: " << filename << ": not a (pingus-config) file");
    return;
  }

  int int_value;
  bool bool_value;
  std::string string_value;
  Size size_value;
    
  if (reader.read_int("master-volume", int_value)) 
  {
    set_master_volume(int_value);
  }
  
  if (reader.read_int("sound-volume", int_value))
  {
    set_sound_volume(int_value);
  }
  
  if (reader.read_int("music-volume", int_value))
  {
    set_music_volume(int_value);
  }
  
  if (reader.read_size("resolution", size_value))
  {
    set_resolution(size_value);
  }

  if (reader.read_bool("fullscreen", bool_value))
  {
    set_fullscreen(bool_value);
  }
  
  if (reader.read_bool("allow-resize", bool_value))
  {
    set_allow_resize(bool_value);
  }

  if (reader.read_bool("mouse-grab", bool_value))
  {
    set_mouse_grab(bool_value);
  }
  
  if (reader.read_bool("print-fps", bool_value))
  {
    set_print_fps(bool_value);
  }
  
  if (reader.read_string("language", string_value))
  {
    set_language(tinygettext::Language::from_spec(string_value));
  }
  
  if (reader.read_bool("software-cursor", bool_value))
  {
    set_swcursor(bool_value);
  }
  
  if (reader.read_bool("autoscroll", bool_value))
  {
    set_autoscroll(bool_value);
  }
}

void
ConfigManager::save()
{
  if (filename.empty())
    filename = System::get_userdir() + "config";

  std::ostringstream out;
  SExprFileWriter writer(out);

  writer.begin_section("pingus-config");

  writer.write_int("master-volume", get_master_volume());
  writer.write_int("sound-volume", get_sound_volume());
  writer.write_int("music-volume", get_music_volume());
  writer.write_size("resolution", get_resolution());
  writer.write_bool("fullscreen", get_fullscreen());
  writer.write_bool("allow-resize", get_allow_resize());
  writer.write_bool("mouse-grab", get_mouse_grab());
  writer.write_bool("print-fps", get_print_fps());
  writer.write_string("language", get_language().str());
  writer.write_bool("software-cursor", get_swcursor());
  writer.write_bool("autoscroll", get_autoscroll());
  
  writer.end_section(); // pingus-config

  out << std::endl;

  System::write_file(filename, out.str());
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
  return 0;
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
  return 0;
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
  return 0;
}

void
ConfigManager::set_resolution(const Size& size)
{
  log_info("ConfigManager::set_resolution: " << size.width << "x" << size.height);

  if (size != get_resolution())
  {
    Display::set_video_mode(size, globals::fullscreen_enabled);
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
ConfigManager::set_swcursor(bool v)
{
  log_info("ConfigManager::set_swcursor: " << v);

  if (v != get_swcursor())
  {
    ScreenManager::instance()->show_swcursor(v);
    on_swcursor_change(v);
  }
}

bool
ConfigManager::get_swcursor()
{
  return globals::swcursor_enabled;
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
