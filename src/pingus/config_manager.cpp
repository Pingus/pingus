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

#include "pingus/config_manager.hpp"

#include "engine/display/display.hpp"
#include "engine/screen/screen_manager.hpp"
#include "pingus/globals.hpp"
#include "pingus/options.hpp"
#include "tinygettext/dictionary_manager.hpp"
#include "util/log.hpp"
#include "util/system.hpp"
#include "engine/sound/sound.hpp"

extern tinygettext::DictionaryManager dictionary_manager;

ConfigManager config_manager;

ConfigManager::ConfigManager() :
  m_opts(),
  on_master_volume_change(),
  on_sound_volume_change(),
  on_music_volume_change(),
  on_fullscreen_resolution_change(),
  on_fullscreen_change(),
  on_renderer_change(),
  on_resizable_change(),
  on_mouse_grab_change(),
  on_print_fps_change(),
  on_language_change(),
  on_software_cursor_change(),
  on_auto_scrolling_change(),
  on_drag_drop_scrolling_change(),
  filename()
{
}

ConfigManager::~ConfigManager()
{
}

void
ConfigManager::set_master_volume(int v)
{
  log_info("ConfigManager::set_master_volume: %1%", v);
  Sound::PingusSound::set_master_volume(static_cast<float>(v) / 100.0f);

  m_opts.master_volume.set(get_master_volume());
}

int
ConfigManager::get_master_volume() const
{
  return static_cast<int>(Sound::PingusSound::get_master_volume() * 100);
}

void
ConfigManager::set_sound_volume(int v)
{
  log_info("ConfigManager::set_sound_volume: %1%", v);
  Sound::PingusSound::set_sound_volume(static_cast<float>(v) / 100.0f);

  m_opts.sound_volume.set(get_sound_volume());
}

int
ConfigManager::get_sound_volume() const
{
  return static_cast<int>(Sound::PingusSound::get_sound_volume() * 100);
}

void
ConfigManager::set_music_volume(int v)
{
  log_info("ConfigManager::set_music_volume: %1%", v);
  Sound::PingusSound::set_music_volume(static_cast<float>(v) / 100.0f);

  m_opts.music_volume.set(get_music_volume());
}

int
ConfigManager::get_music_volume() const
{
  return static_cast<int>(Sound::PingusSound::get_music_volume() * 100);
}

void
ConfigManager::set_fullscreen_resolution(const Size& size)
{
  log_info("%1%x%2%", size.width, size.height);

  if (size != get_fullscreen_resolution())
  {
    if (Display::is_fullscreen())
    {
      Display::set_video_mode(size, Display::is_fullscreen(), false);
    }
    on_fullscreen_resolution_change(size);
  }

  m_opts.fullscreen_resolution.set(size);
}

Size
ConfigManager::get_fullscreen_resolution() const
{
  return m_opts.fullscreen_resolution.get();
}

void
ConfigManager::set_fullscreen(bool v)
{
  log_info("ConfigManager::set_fullscreen: %1%", v);

  if (v != get_fullscreen())
  {
    Display::set_video_mode(Display::get_size(), v, false);
    on_fullscreen_change(v);
  }

  m_opts.fullscreen.set(v);
}

bool
ConfigManager::get_fullscreen() const
{
  return m_opts.fullscreen.get();
}

void
ConfigManager::set_renderer(FramebufferType type)
{
  return m_opts.framebuffer_type.set(type);
}

FramebufferType
ConfigManager::get_renderer() const
{
  return m_opts.framebuffer_type.get();
}

void
ConfigManager::set_resizable(bool v)
{
  log_info("ConfigManager::set_resizable: %1%", v);

  if (v != get_resizable())
  {
    on_resizable_change(v);
  }

  m_opts.resizable.set(v);
}

bool
ConfigManager::get_resizable() const
{
  return m_opts.resizable.get();
}

void
ConfigManager::set_mouse_grab(bool v)
{
  log_info("ConfigManager::set_mouse_grab: %1%", v);

  if (v != get_mouse_grab())
  {
    SDL_SetWindowGrab(SDL_GL_GetCurrentWindow(), static_cast<SDL_bool>(v));
    on_mouse_grab_change(v);
  }

  m_opts.mouse_grab.set(v);
}

bool
ConfigManager::get_mouse_grab() const
{
  return SDL_GetWindowGrab(SDL_GL_GetCurrentWindow());
}

void
ConfigManager::set_print_fps(bool v)
{
  log_info("ConfigManager::set_print_fps: %1%", v);

  if (v != get_print_fps())
  {
    globals::print_fps = v;
    on_print_fps_change(v);
  }

  m_opts.print_fps.set(v);
}

bool
ConfigManager::get_print_fps() const
{
  return globals::print_fps;
}

void
ConfigManager::set_language(const tinygettext::Language& v)
{
  log_info("%1%", v.str());

  if (v != get_language())
  {
    dictionary_manager.set_language(v);
    on_language_change(v);
  }

  m_opts.language.set(v.str());
}

tinygettext::Language
ConfigManager::get_language() const
{
  return dictionary_manager.get_language();
}

void
ConfigManager::set_software_cursor(bool v)
{
  log_info("ConfigManager::set_software_cursor: %1%", v);

  if (v != get_software_cursor())
  {
    ScreenManager::instance()->show_software_cursor(v);
    on_software_cursor_change(v);
  }

  m_opts.software_cursor.set(v);
}

bool
ConfigManager::get_software_cursor() const
{
  return globals::software_cursor;
}

void
ConfigManager::set_auto_scrolling(bool v)
{
  log_info("ConfigManager::set_auto_scrolling: %1%", v);

  if (v != get_auto_scrolling())
  {
    globals::auto_scrolling = v;
    on_auto_scrolling_change(v);
  }

  m_opts.auto_scrolling.set(v);
}

bool
ConfigManager::get_auto_scrolling() const
{
  return globals::auto_scrolling;
}

void
ConfigManager::set_drag_drop_scrolling(bool v)
{
  if (globals::drag_drop_scrolling != v)
  {
    globals::drag_drop_scrolling = v;
    m_opts.drag_drop_scrolling.set(v);
    on_drag_drop_scrolling_change(v);
  }
}

bool
ConfigManager::get_drag_drop_scrolling() const
{
  return globals::drag_drop_scrolling;
}

Options
ConfigManager::get_options() const
{
  return m_opts;
}

void
ConfigManager::apply(const Options& opts)
{
  m_opts.merge(opts);

  if (m_opts.framebuffer_type.is_set())
    set_renderer(m_opts.framebuffer_type.get());
  else
    set_renderer(SDL_FRAMEBUFFER);

  if (opts.master_volume.is_set())
    set_master_volume(opts.master_volume.get());

  if (opts.sound_volume.is_set())
    set_sound_volume(opts.sound_volume.get());

  if (opts.music_volume.is_set())
    set_music_volume(opts.music_volume.get());

  if (opts.fullscreen_resolution.is_set())
    set_fullscreen_resolution(opts.fullscreen_resolution.get());

  if (opts.fullscreen.is_set())
    set_fullscreen(opts.fullscreen.get());

  if (opts.resizable.is_set())
    set_resizable(opts.resizable.get());

  if (opts.mouse_grab.is_set())
    set_mouse_grab(opts.mouse_grab.get());

  if (opts.print_fps.is_set())
    set_print_fps(opts.print_fps.get());

  if (opts.software_cursor.is_set())
    set_software_cursor(opts.software_cursor.get());

  if (opts.auto_scrolling.is_set())
    set_auto_scrolling(opts.auto_scrolling.get());

  if (opts.language.is_set())
    set_language(tinygettext::Language::from_env(opts.language.get()));
}

/* EOF */
