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

#ifndef HEADER_EVDEV_DEVICE_HPP
#define HEADER_EVDEV_DEVICE_HPP

#include <vector>
#include <string>

namespace Input {

/** */
class EvdevDevice
{
private:
  // Absolute Input Event
  struct Absolute {
    Absolute(int code_, int min_, int max_, int pos_)
      : code(code_), min(min_), max(max_), pos(pos_) {}

    int code;
    int min;
    int max;
    int pos;

    inline float get_posf() const {
      return float(pos - min)/(max - min) * 2.0f - 1.0f;
    }
  };
	
  // Relative Input Event
  struct Relative {
    Relative(int code_) : code(code_), pos(0) {}
    int code;
    int pos;
  };

  // Key Input Event
  struct Key {
    Key(int code_) : code(code_), pressed(false) {}
    int code;
    bool pressed;
  };

  int fd;
  int version;

  std::vector<Relative>   relatives;
  std::vector<Absolute>   absolutes;
  std::vector<Key> keys;

  std::string name;
  std::string device;

public:
  EvdevDevice(const std::string& filename);
  ~EvdevDevice();
  
  void update(float delta);
  void process_absolute(struct input_event& ev);
  void process_relative(struct input_event& ev);
  void process_key(struct input_event& ev);

  int get_key_index_by_code(int code);
  int get_relative_index_by_code(int code);
  int get_absolute_index_by_code(int code);
};

} // namespace Input

#endif

/* EOF */
