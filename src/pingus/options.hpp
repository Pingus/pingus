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

#ifndef HEADER_PINGUS_PINGUS_OPTIONS_HPP
#define HEADER_PINGUS_PINGUS_OPTIONS_HPP

#include <string>

#include "math/size.hpp"
#include "util/pathname.hpp"

class ReaderObject;
class ReaderMapping;

template<class T>
class Value
{
private:
  T    value;
  bool valid;

public:
  Value()
    : value(T()),
      valid(false)
  {}

  Value(const T& v)
    : value(v),
      valid(true)
  {}

  const T& get() const {
    return value;
  }

  void set(const T& v) {
    value = v;
    valid = true;
  }

  bool is_set() const {
    return valid;
  }

  void merge(const Value<T>& rhs)
  {
    if (rhs.is_set())
    {
      value = rhs.get();
      valid = true;
    }
  }
};

enum FramebufferType
{
 DELTA_FRAMEBUFFER,
 SDL_FRAMEBUFFER,
 OPENGL_FRAMEBUFFER,
 NULL_FRAMEBUFFER
};

std::string framebuffer_type_to_string(FramebufferType type);
FramebufferType framebuffer_type_from_string(const std::string& str);

struct Options
{
  static Options from_file_reader(const ReaderMapping& reader);
  static Options from_file(const Pathname& filename);

  // Display
  Value<FramebufferType> framebuffer_type;
  Value<bool> fullscreen;
  Value<bool> resizable;
  Value<bool> software_cursor;
  Value<bool> mouse_grab;
  Value<Size> geometry;
  Value<Size> fullscreen_resolution;

  // Sound
  Value<int>  master_volume;
  Value<int>  sound_volume;
  Value<int>  music_volume;
  Value<bool> disable_sound;
  Value<bool> disable_music;

  // Misc
  Value<std::string> language;
  Value<std::string> datadir;
  Value<std::string> userdir;

  Value<bool> auto_scrolling;
  Value<bool> drag_drop_scrolling;
  Value<std::string> controller;

  Value<bool> developer_mode;

  Value<bool>  print_fps;
  Value<int>   speed;
  Value<float> desiredfps;
  Value<int>   tile_size;

  Options() :
    framebuffer_type(),
    fullscreen(),
    resizable(),
    software_cursor(),
    mouse_grab(),
    geometry(),
    fullscreen_resolution(),
    master_volume(),
    sound_volume(),
    music_volume(),
    disable_sound(),
    disable_music(),
    language(),
    datadir(),
    userdir(),
    auto_scrolling(),
    drag_drop_scrolling(),
    controller(),
    developer_mode(),
    print_fps(),
    speed(),
    desiredfps(),
    tile_size()
  {}

  virtual ~Options() {}

  void merge(const Options& rhs);

  virtual void save(const Pathname& filename) const;
};

struct CommandLineOptions : public Options
{
  Value<std::string> rest;

  // Modes
  Value<bool> list_languages;
  Value<bool> editor;
  Value<bool> no_config_file;

  CommandLineOptions() :
    rest(),
    list_languages(),
    editor(),
    no_config_file()
  {}

  virtual ~CommandLineOptions() {}

  void merge(const Options& rhs) { Options::merge(rhs); }
  void merge(const CommandLineOptions& rhs);
};

#endif

/* EOF */
