//  $Id: joystick_axis.cxx,v 1.1 2002/07/04 14:03:43 torangan Exp $
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

namespace Input
{
  JoystickAxis::JoystickAxis(int id_, int axis_, float angle_) : id(id_), axis(axis_), angle(angle_) { }


  float
  JoystickAxis::get_pos ()
  {
    return CL_Input::joysticks[id]->get_axis(axis)->get_pos();
  }

  float
  JoystickAxis::get_angle ()
  {
    return angle;
  }
  
  void
  JoystickAxis::update(float)
  {
  }
}

/* EOF */
