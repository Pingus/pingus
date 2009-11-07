//  Pingus - A free Lemmings clone
//  Copyright (C) 1998-2009 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_ENGINE_INPUT_USBMOUSE_USBMOUSE_HPP
#define HEADER_PINGUS_ENGINE_INPUT_USBMOUSE_USBMOUSE_HPP

#include <vector>

#include "pingus/globals.hpp"
#include "pingus/debug.hpp"
#include "math/vector2i.hpp"

namespace Input {

class USBMouse
{
private:
  int fd;
  Vector2i mouse_pos;

  std::string device;
  std::vector<bool> buttons;

  std::vector<Pointer*>  pointer_bindings;
  std::vector<Scroller*> scroller_bindings;
  std::vector<std::vector<Button*> >  button_bindings;
  
public: 
  USBMouse(const std::string& device_);
  ~USBMouse();

  void add_listener(Pointer* p);
  void add_listener(Scroller* s);
  void add_listener(int i, Button* b);

  std::string get_device() const;
  void update(float delta);
};

} // namespace Input

#endif

/* EOF */
