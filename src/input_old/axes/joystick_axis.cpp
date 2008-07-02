//  $Id: joystick_axis.cpp 3059 2007-09-02 03:48:52Z grumbel $
//
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

#include "joystick_axis.hpp"
#include "../../pingus_error.hpp"

namespace Input {
namespace Axes {

JoystickAxis::JoystickAxis(int id_, int axis_, float angle_) 
  : id(id_), axis(axis_), pos(0), angle(angle_)
{
#if 0
  if (id >= CL_Joystick::get_device_count())
    PingusError::raise("JoystickAxis: Invalid joystick id");

  if (axis > CL_Joystick::get_device(id).get_axis_count())
    PingusError::raise("JoystickAxis: Invalid joystick axis id");
#endif

  if (angle < 0)
    angle = float(static_cast<int>(angle) % 360) + 360;
  else if (angle > 360)
    angle = float(static_cast<int>(angle) % 360);
}

const float&
JoystickAxis::get_pos () const
{
  return pos;
}

const float&
JoystickAxis::get_angle () const
{
  return angle;
}

void
JoystickAxis::update (float)
{
#if 0
  pos = CL_Joystick::get_device(id).get_axis(axis);
#endif
}

} // namespace Axes
} // namespace Input

/* EOF */
