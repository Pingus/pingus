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

#ifndef HEADER_PINGUS_ENGINE_INPUT_XINPUT_XINPUT_DEVICE_HPP
#define HEADER_PINGUS_ENGINE_INPUT_XINPUT_XINPUT_DEVICE_HPP

#include <X11/extensions/XInput.h>
#include <string>
#include <vector>

#include "math/vector2i.hpp"

namespace Input {

class XInputDriver;

class XInputDevice
{
private:
  struct AxisInfo {
    int min_value;
    int max_value;
    int resolution;
    int pos;

    AxisInfo(int min_value_, int max_value_, int resolution_)
      : min_value(min_value_), max_value(max_value_), resolution(resolution_), pos(0)
    {}
  };

private:
  XInputDriver* owner;
  std::string name;
    
  bool absolute;
    
  std::vector<bool> buttons;
  std::vector<AxisInfo> axis;

  int num_keys; 
  
  Vector2i mouse_pos;
  Time time_at_last_press;
  int  last_press_id;
  
  bool key_states[5];

  int motion_type;
  int button_press_type;
  int button_release_type;
  int key_press_type;
  int key_release_type;
  int proximity_in_type;
  int proximity_out_type;

public:
  XInputDevice(XInputDriver* owner_, XDeviceInfo* info);
  ~XInputDevice();

  void get_info(XDeviceInfo* info);
  void on_xevent(XEvent &event);
  int  register_events(Display* dpy,
                       XDeviceInfo* info,
                       const char* dev_name,
                       Bool handle_proximity);

private:
  XInputDevice(const XInputDevice&);
  XInputDevice & operator=(const XInputDevice&);
};

} // namespace Input

#endif

/* EOF */
