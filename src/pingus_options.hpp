//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef HEADER_OPTIONS_HPP
#define HEADER_OPTIONS_HPP

#include <string>
#include "math/size.hpp"
#include "pathname.hpp"

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
};

struct PingusOptions
{
  Value<std::string> rest;

  // Modes
  Value<bool> list_languages;
  Value<bool> editor;
  Value<bool> credits;
  Value<bool> font;
  Value<std::string> save;

  // Display
  Value<bool> fullscreen;
  Value<bool> resize;
  Value<bool> swcursor;
  Value<Size> geometry;

  // Sound
  Value<bool> disable_sound;
  Value<bool> disable_music;

  // Misc
  Value<std::string> language;
  Value<std::string> datadir;

  Value<bool> auto_scrolling;
  Value<bool> no_config_file;
  Value<std::string> config_file;
  Value<std::string> controller;

  Value<bool> maintainer_mode;
  Value<unsigned int> debug;

  Value<int>  min_frame_skip;
  Value<int>  max_frame_skip;
  Value<int>  speed;
  Value<float> desiredfps;
  Value<int>  tile_size;
  Value<bool> fast_mode;
};

#endif

/* EOF */
