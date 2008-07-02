
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

#ifndef HEADER_INPUT_XINPUT_DRIVER_HPP
#define HEADER_INPUT_XINPUT_DRIVER_HPP

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XInput.h>
#include <vector>

#include "SDL_syswm.h"
#include "driver.hpp"

namespace Input {

class XInputDevice;

/** */
class XInputDriver : public Driver
{
private:
  SDL_SysWMinfo sys;

  void setup_xinput();
  Bool xinput_is_present();

  std::vector<XInputDevice*> devices;


  XDeviceInfo* find_device_info(Display *display,
                                const char* name,
                                Bool only_extended);
public:
  XInputDriver();
  ~XInputDriver();

  SDL_SysWMinfo& get_syswminfo() { return sys; }
 
  std::string get_name() const { return "xinput"; }
  void update(float delta);

  Button*   create_button  (const FileReader& reader, Control* parent);
  Axis*     create_axis    (const FileReader& reader, Control* parent);
  Scroller* create_scroller(const FileReader& reader, Control* parent);
  Pointer*  create_pointer (const FileReader& reader, Control* parent);
  Keyboard* create_keyboard(const FileReader& reader, Control* parent) { return 0; }
};

} // namespace Input

#endif

/* EOF */
