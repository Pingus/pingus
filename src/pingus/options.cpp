// Pingus - A free Lemmings clone
// Copyright (C) 1998-2011 Ingo Ruhnke <grumbel@gmail.com>
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

#include "pingus/options.hpp"

#include <stdexcept>

#include "tinygettext/language.hpp"
#include "util/reader.hpp"
#include "util/log.hpp"
#include "util/pathname.hpp"
#include "util/raise_exception.hpp"
#include "util/reader.hpp"
#include "util/writer.hpp"
#include "util/system.hpp"

std::string framebuffer_type_to_string(FramebufferType type)
{
  switch(type)
  {
    case DELTA_FRAMEBUFFER:
      return "delta";

    case SDL_FRAMEBUFFER:
      return "sdl";

    case NULL_FRAMEBUFFER:
      return "null";

    case OPENGL_FRAMEBUFFER:
      return "opengl";

    default:
      log_error("unknown FramebufferType: %1%", static_cast<int>(type));
      return "sdl";
  }
}

FramebufferType framebuffer_type_from_string(const std::string& str)
{
  if (str == "delta")
  {
    return DELTA_FRAMEBUFFER;
  }
  else if (str == "sdl")
  {
    return SDL_FRAMEBUFFER;
  }
  else if (str == "null")
  {
    return NULL_FRAMEBUFFER;
  }
  else if (str == "opengl")
  {
    return OPENGL_FRAMEBUFFER;
  }
  else
  {
    log_error("unknown FramebufferType '%1%', default to 'sdl'", str);
    return SDL_FRAMEBUFFER;
  }
}

Options
Options::from_file(const Pathname& filename)
{
  ReaderObject reader = Reader::parse(filename);

  if (reader.get_name() != "pingus-config")
  {
    raise_exception(std::runtime_error, "Error: " << filename << ": not a (pingus-config) file");
  }

  return from_file_reader(reader.get_mapping());
}

Options
Options::from_file_reader(const ReaderMapping& reader)
{
  Options opts;

  int int_value;
  bool bool_value;
  std::string string_value;
  Size size_value;

  FramebufferType fbtype_value = SDL_FRAMEBUFFER;

  if (reader.read_enum("renderer", fbtype_value, framebuffer_type_from_string))
  {
    opts.framebuffer_type.set(fbtype_value);
  }

  if (reader.read_int("master-volume", int_value))
  {
    opts.master_volume.set(int_value);
  }

  if (reader.read_int("sound-volume", int_value))
  {
    opts.sound_volume.set(int_value);
  }

  if (reader.read_int("music-volume", int_value))
  {
    opts.music_volume.set(int_value);
  }

  if (reader.read_size("geometry", size_value))
  {
    opts.geometry.set(size_value);
  }

  if (reader.read_size("fullscreen-resolution", size_value))
  {
    opts.fullscreen_resolution.set(size_value);
  }

  if (reader.read_bool("fullscreen", bool_value))
  {
    opts.fullscreen.set(bool_value);
  }

  if (reader.read_bool("resizable", bool_value))
  {
    opts.resizable.set(bool_value);
  }

  if (reader.read_bool("mouse-grab", bool_value))
  {
    opts.mouse_grab.set(bool_value);
  }

  if (reader.read_bool("print-fps", bool_value))
  {
    opts.mouse_grab.set(bool_value);
  }

  if (reader.read_string("controller", string_value))
  {
    opts.controller.set(string_value);
  }

  if (reader.read_string("language", string_value))
  {
    opts.language.set(string_value);
  }

  if (reader.read_bool("software-cursor", bool_value))
  {
    opts.software_cursor.set(bool_value);
  }

  if (reader.read_bool("auto-scrolling", bool_value))
  {
    opts.auto_scrolling.set(bool_value);
  }

  if (reader.read_bool("drag-drop-scrolling", bool_value))
  {
    opts.drag_drop_scrolling.set(bool_value);
  }

  return opts;
}

void
Options::save(const Pathname& filename) const
{
  std::ostringstream out;
  Writer writer(out);

  writer.begin_object("pingus-config");

  if (framebuffer_type.is_set())
    writer.write_enum("renderer", framebuffer_type.get(), framebuffer_type_to_string);

  if (master_volume.is_set())
    writer.write_int("master-volume", master_volume.get());

  if (sound_volume.is_set())
    writer.write_int("sound-volume", sound_volume.get());

  if (music_volume.is_set())
    writer.write_int("music-volume", music_volume.get());

  if (geometry.is_set())
    writer.write_size("geometry", geometry.get());

  if (fullscreen_resolution.is_set())
    writer.write_size("fullscreen-resolution", fullscreen_resolution.get());

  if (fullscreen.is_set())
    writer.write_bool("fullscreen", fullscreen.get());

  if (resizable.is_set())
    writer.write_bool("resizable", resizable.get());

  if (mouse_grab.is_set())
    writer.write_bool("mouse-grab", mouse_grab.get());

  if (print_fps.is_set())
    writer.write_bool("print-fps", print_fps.get());

  if (controller.is_set())
    writer.write_string("controller", controller.get());

  if (language.is_set())
    writer.write_string("language", language.get());

  if (software_cursor.is_set())
    writer.write_bool("software-cursor", software_cursor.get());

  if (auto_scrolling.is_set())
    writer.write_bool("auto-scrolling", auto_scrolling.get());

  if (drag_drop_scrolling.is_set())
    writer.write_bool("drag-drop-scrolling", drag_drop_scrolling.get());

  writer.end_object(); // pingus-config

  out << std::endl;

  System::write_file(filename.get_sys_path(), out.str());
}

void
Options::merge(const Options& rhs)
{
  // Display
  framebuffer_type.merge(rhs.framebuffer_type);
  fullscreen.merge(rhs.fullscreen);
  resizable.merge(rhs.resizable);
  software_cursor.merge(rhs.software_cursor);
  mouse_grab.merge(rhs.mouse_grab);
  geometry.merge(rhs.geometry);
  fullscreen_resolution.merge(rhs.fullscreen_resolution);

  // Sound
  master_volume.merge(rhs.master_volume);
  sound_volume.merge(rhs.sound_volume);
  music_volume.merge(rhs.music_volume);
  disable_sound.merge(rhs.disable_sound);
  disable_music.merge(rhs.disable_music);

  // Misc
  language.merge(rhs.language);
  datadir.merge(rhs.datadir);
  userdir.merge(rhs.userdir);

  auto_scrolling.merge(rhs.auto_scrolling);
  drag_drop_scrolling.merge(rhs.drag_drop_scrolling);
  controller.merge(rhs.controller);

  developer_mode.merge(rhs.developer_mode);

  print_fps.merge(rhs.print_fps);
  speed.merge(rhs.speed);
  desiredfps.merge(rhs.desiredfps);
  tile_size.merge(rhs.tile_size);
}

void
CommandLineOptions::merge(const CommandLineOptions& rhs)
{
  Options::merge(rhs);

  rest.merge(rhs.rest);
  list_languages.merge(rhs.list_languages);
  editor.merge(rhs.editor);
  no_config_file.merge(rhs.no_config_file);
}

/* EOF */
