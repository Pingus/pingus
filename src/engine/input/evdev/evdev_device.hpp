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

#ifndef HEADER_PINGUS_ENGINE_INPUT_EVDEV_EVDEV_DEVICE_HPP
#define HEADER_PINGUS_ENGINE_INPUT_EVDEV_EVDEV_DEVICE_HPP

#include <linux/input.h>
#include <string.h>

#include "engine/input/control.hpp"

namespace Input {

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
      return static_cast<float>(pos - min) / static_cast<float>(max - min) * 2.0f - 1.0f;
    }
  };
        
  // Relative Input Event
  struct Relative 
  {
    Relative(int code_) : 
      code(code_),
      pos(0),
      binding_axis(),
      bindings()
    {}

    int code;
    int pos;

    int binding_axis; // FIXME: Should be part of the binding
    std::vector<Scroller*> bindings;
  };

  // Key Input Event
  struct Key {
    Key(int code_) : code(code_), pressed(false), bindings() {}
    int code;
    bool pressed;
    std::vector<Button*> bindings;
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
  
  std::string get_name() const { return name; }
  std::string get_device() const { return device; }

  void update(float delta);
  void process_absolute(struct input_event& ev);
  void process_relative(struct input_event& ev);
  void process_key(struct input_event& ev);

  int get_key_index_by_code(int code);
  int get_relative_index_by_code(int code);
  int get_absolute_index_by_code(int code);

  Scroller* create_scroller(Control* parent, int x, int y);
  Button*   create_button(Control* parent, int id);
};

} // namespace Input

#endif

/* EOF */
