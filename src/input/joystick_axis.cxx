//  $Id: joystick_axis.cxx,v 1.6 2002/08/15 10:57:16 torangan Exp $
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

#include <ClanLib/Display/Input/input.h>
#include <ClanLib/Display/Input/inputdevice.h>
#include <ClanLib/Display/Input/inputaxis.h>
#include "joystick_axis.hxx"
#include "../pingus_error.hxx"

namespace Input
{
  JoystickAxis::JoystickAxis(int id_, int axis_, float angle_) : id(id_), axis(axis_), pos(0), angle(angle_)
  {
    if (static_cast<unsigned int> (id) >= CL_Input::joysticks.size())
      throw PingusError("JoystickAxis: Invalid joystick id");
      
    if (axis > CL_Input::joysticks[id]->get_num_axes())
      throw PingusError("JoystickAxis: Invalid joystick axis id");
  
    if (angle < 0)
      angle = (static_cast<int>(angle) % 360) + 360;
    else if (angle > 360)
      angle = static_cast<int>(angle) % 360;
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
    pos = CL_Input::joysticks[id]->get_axis(axis)->get_pos();
  }
}

/* EOF */
