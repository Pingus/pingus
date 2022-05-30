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

#include <logmich/log.hpp>

#include "util/writer.hpp"
#include "util/reader.hpp"
#include "tinygettext/language.hpp"
#include "util/pathname.hpp"
#include "util/raise_exception.hpp"
#include "util/system.hpp"

namespace pingus {

std::string framebuffer_type_to_string(FramebufferType type)
{
  switch(type)
  {
    case SDL_FRAMEBUFFER:
      return "sdl";

    case NULL_FRAMEBUFFER:
      return "null";

    case OPENGL_FRAMEBUFFER:
      return "opengl";

    default:
      log_error("unknown FramebufferType: {}", static_cast<int>(type));
      return "sdl";
  }
}

FramebufferType framebuffer_type_from_string(std::string const& str)
{
  if (str == "sdl")
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
    log_error("unknown FramebufferType '{}', default to 'sdl'", str);
    return SDL_FRAMEBUFFER;
  }
}

Options
Options::from_file(Pathname const& filename)
{
  auto doc = prio::ReaderDocument::from_file(filename.get_sys_path());

  if (doc.get_name() != "pingus-config")
  {
    raise_exception(std::runtime_error, "Error: " << filename << ": not a (pingus-config) file");
  }

  return from_file_reader(doc.get_mapping());
}

Options
Options::from_file_reader(ReaderMapping const& reader)
{
  Options opts;

  int int_value;
  bool bool_value;
  std::string string_value;
  Size size_value;

  FramebufferType fbtype_value = SDL_FRAMEBUFFER;

  if (reader.read("renderer", fbtype_value, framebuffer_type_from_string))
  {
    opts.framebuffer_type.set(fbtype_value);
  }

  if (reader.read("master-volume", int_value))
  {
    opts.master_volume.set(int_value);
  }

  if (reader.read("sound-volume", int_value))
  {
    opts.sound_volume.set(int_value);
  }

  if (reader.read("music-volume", int_value))
  {
    opts.music_volume.set(int_value);
  }

  if (reader.read("geometry", size_value))
  {
    opts.geometry.set(size_value);
  }

  if (reader.read("fullscreen-resolution", size_value))
  {
    opts.fullscreen_resolution.set(size_value);
  }

  if (reader.read("fullscreen", bool_value))
  {
    opts.fullscreen.set(bool_value);
  }

  if (reader.read("resizable", bool_value))
  {
    opts.resizable.set(bool_value);
  }

  if (reader.read("mouse-grab", bool_value))
  {
    opts.mouse_grab.set(bool_value);
  }

  if (reader.read("print-fps", bool_value))
  {
    opts.mouse_grab.set(bool_value);
  }

  if (reader.read("controller", string_value))
  {
    opts.controller.set(string_value);
  }

  if (reader.read("language", string_value))
  {
    opts.language.set(string_value);
  }

  if (reader.read("software-cursor", bool_value))
  {
    opts.software_cursor.set(bool_value);
  }

  if (reader.read("auto-scrolling", bool_value))
  {
    opts.auto_scrolling.set(bool_value);
  }

  if (reader.read("drag-drop-scrolling", bool_value))
  {
    opts.drag_drop_scrolling.set(bool_value);
  }

  return opts;
}

void
Options::save(Pathname const& filename) const
{
  std::ostringstream out;
  Writer writer(out);

  writer.begin_object("pingus-config");

  if (framebuffer_type.is_set())
    writer.write("renderer", framebuffer_type.get(), framebuffer_type_to_string);

  if (master_volume.is_set())
    writer.write("master-volume", master_volume.get());

  if (sound_volume.is_set())
    writer.write("sound-volume", sound_volume.get());

  if (music_volume.is_set())
    writer.write("music-volume", music_volume.get());

  if (geometry.is_set())
    writer.write("geometry", geometry.get());

  if (fullscreen_resolution.is_set())
    writer.write("fullscreen-resolution", fullscreen_resolution.get());

  if (fullscreen.is_set())
    writer.write("fullscreen", fullscreen.get());

  if (resizable.is_set())
    writer.write("resizable", resizable.get());

  if (mouse_grab.is_set())
    writer.write("mouse-grab", mouse_grab.get());

  if (print_fps.is_set())
    writer.write("print-fps", print_fps.get());

  if (controller.is_set())
    writer.write("controller", controller.get());

  if (language.is_set())
    writer.write("language", language.get());

  if (software_cursor.is_set())
    writer.write("software-cursor", software_cursor.get());

  if (auto_scrolling.is_set())
    writer.write("auto-scrolling", auto_scrolling.get());

  if (drag_drop_scrolling.is_set())
    writer.write("drag-drop-scrolling", drag_drop_scrolling.get());

  writer.end_object(); // pingus-config

  out << std::endl;

  System::write_file(filename.get_sys_path(), out.str());
}

void
Options::merge(Options const& rhs)
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
CommandLineOptions::merge(CommandLineOptions const& rhs)
{
  Options::merge(rhs);

  rest.merge(rhs.rest);
  list_languages.merge(rhs.list_languages);
  editor.merge(rhs.editor);
  no_config_file.merge(rhs.no_config_file);
}

} // namespace pingus

/* EOF */
