//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#include "../../pingus_error.hpp"
#include "joystick_button.hpp"

namespace Input {
namespace Buttons {

JoystickButton::JoystickButton(int id_, int button_) : id(id_), button(button_)
{
#if 0
  if (id >= CL_Joystick::get_device_count())
    throw std::runtime_error("JoystickButton: Invalid joystick id: " + CL_String::to(id));

  if (button > CL_Joystick::get_device(id).get_button_count())
    throw std::runtime_error("JoystickButton: Invalid joystick button id: " + CL_String::to(button));
#endif
}

void
JoystickButton::update(float)
{
}

bool
JoystickButton::is_pressed() const
{
  //  return CL_Joystick::get_device(id).get_keycode(button);
  return false;
}

} // namespace Buttons
} // namespace Input

/* EOF */
