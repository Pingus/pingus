// Pingus - A free Lemmings clone
// Copyright (C) 2007 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_PINGUS_CONFIG_MANAGER_HPP
#define HEADER_PINGUS_PINGUS_CONFIG_MANAGER_HPP

#include <boost/signals2.hpp>

#include "math/size.hpp"
#include "pingus/options.hpp"
#include "tinygettext/language.hpp"

struct Options;

class ConfigManager
{
private:
  Options m_opts;

public:
  ConfigManager();
  ~ConfigManager();

  void apply(const Options& opts);
  Options get_options() const;

  void set_master_volume(int);
  int  get_master_volume() const;
  boost::signals2::signal<void(int)> on_master_volume_change;

  void set_sound_volume(int);
  int  get_sound_volume() const;
  boost::signals2::signal<void(int)> on_sound_volume_change;

  void set_music_volume(int);
  int  get_music_volume() const;
  boost::signals2::signal<void(int)> on_music_volume_change;

  void set_fullscreen_resolution(const Size& size);
  Size get_fullscreen_resolution() const;
  boost::signals2::signal<void(Size)> on_fullscreen_resolution_change;

  void set_fullscreen(bool);
  bool get_fullscreen() const;
  boost::signals2::signal<void(bool)> on_fullscreen_change;

  void set_renderer(FramebufferType type);
  FramebufferType get_renderer() const;
  boost::signals2::signal<void(FramebufferType)> on_renderer_change;

  void set_resizable(bool);
  bool get_resizable() const;
  boost::signals2::signal<void(bool)> on_resizable_change;

  void set_mouse_grab(bool);
  bool get_mouse_grab() const;
  boost::signals2::signal<void(bool)> on_mouse_grab_change;

  void set_print_fps(bool);
  bool get_print_fps() const;
  boost::signals2::signal<void(bool)> on_print_fps_change;

  void set_language(const tinygettext::Language&);
  tinygettext::Language get_language() const;
  boost::signals2::signal<void(const tinygettext::Language&)> on_language_change;

  void set_software_cursor(bool);
  bool get_software_cursor() const;
  boost::signals2::signal<void(bool)> on_software_cursor_change;

  void set_auto_scrolling(bool);
  bool get_auto_scrolling() const;
  boost::signals2::signal<void(bool)> on_auto_scrolling_change;

  void set_drag_drop_scrolling(bool);
  bool get_drag_drop_scrolling() const;
  boost::signals2::signal<void(bool)> on_drag_drop_scrolling_change;

private:
  ConfigManager (const ConfigManager&);
  ConfigManager& operator= (const ConfigManager&);

  std::string filename;
};

extern ConfigManager config_manager;

#endif

/* EOF */
