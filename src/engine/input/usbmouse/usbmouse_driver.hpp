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

#ifndef HEADER_PINGUS_ENGINE_INPUT_USBMOUSE_USBMOUSE_DRIVER_HPP
#define HEADER_PINGUS_ENGINE_INPUT_USBMOUSE_USBMOUSE_DRIVER_HPP

//#include <vector>

#include "engine/input/driver.hpp"

namespace Input {

class USBMouse;

/** 
 */
class USBMouseDriver : public Driver
{
private:
  typedef std::vector<USBMouse*> USBMice;
  USBMice usbmice;

public:
  USBMouseDriver();
  ~USBMouseDriver();

  std::string get_name() const { return "usbmouse"; }

  void update(float delta);
  
  Button*   create_button  (const FileReader& reader, Control* parent);
  Axis*     create_axis    (const FileReader& reader, Control* parent) { return 0; } 
  Scroller* create_scroller(const FileReader& reader, Control* parent);
  Pointer*  create_pointer (const FileReader& reader, Control* parent);
  Keyboard*  create_keyboard(const FileReader& reader, Control* parent) { return 0; }
private:
  USBMouse* get_mouse(const std::string& device);
};

} // namespace Input

#endif

/* EOF */
