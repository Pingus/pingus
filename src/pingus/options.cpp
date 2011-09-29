//  Pingus - A free Lemmings clone
//  Copyright (C) 1998-2011 Ingo Ruhnke <grumbel@gmx.de>
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

#include "pingus/options.hpp"

#include <stdexcept>

#include "tinygettext/language.hpp"
#include "util/file_reader.hpp"
#include "util/log.hpp"
#include "util/pathname.hpp"
#include "util/raise_exception.hpp"
#include "util/sexpr_file_reader.hpp"
#include "util/sexpr_file_writer.hpp"
#include "util/system.hpp"

Options
Options::from_file(const Pathname& filename)
{
  FileReader reader = FileReader::parse(filename);

  if (reader.get_name() != "pingus-config")
  {
    raise_exception(std::runtime_error, "Error: " << filename << ": not a (pingus-config) file");
  }

  return from_file_reader(reader);
}

Options
Options::from_file_reader(const FileReader& reader)
{
  Options opts;

  int int_value;
  bool bool_value;
  std::string string_value;
  Size size_value;
    
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
  
  if (reader.read_bool("allow-resize", bool_value))
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
  
  if (reader.read_string("language", string_value))
  {
    //FIXME: set_language(tinygettext::Language::from_spec(string_value));
  }
  
  if (reader.read_bool("software-cursor", bool_value))
  {
    opts.software_cursor.set(bool_value);
  }
  
  if (reader.read_bool("autoscroll", bool_value))
  {
    opts.auto_scrolling.set(bool_value);
  }

  return opts;
}

void
Options::save(const Pathname& filename) const
{
  std::ostringstream out;
  SExprFileWriter writer(out);

  writer.begin_section("pingus-config");

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
    writer.write_bool("allow-resize", resizable.get());

  if (mouse_grab.is_set())
    writer.write_bool("mouse-grab", mouse_grab.get());

  if (print_fps.is_set())
    writer.write_bool("print-fps", print_fps.get());

  // FIXME: if (language.is_set())
  //  writer.write_string("language", language.get().str.get());

  if (software_cursor.is_set())
    writer.write_bool("software-cursor", software_cursor.get());

  if (auto_scrolling.is_set())
    writer.write_bool("autoscroll", auto_scrolling.get());
  
  writer.end_section(); // pingus-config

  out << std::endl;

  System::write_file(filename.get_sys_path(), out.str());
}

void
Options::merge(const Options& rhs)
{
  // Display
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
