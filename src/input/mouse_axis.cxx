//  $Id: mouse_axis.cxx,v 1.1 2002/07/08 14:47:35 torangan Exp $
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
#include "mouse_axis.hxx"

namespace Input
{
  MouseAxis::MouseAxis(int axis_, float angle_) : axis(axis_), angle(angle_)
  {
    if (angle < 0)
      angle = (static_cast<int>(angle) % 360) + 360;
    else if (angle > 360)
      angle = static_cast<int>(angle) % 360;
  }

  float
  MouseAxis::get_pos ()
  {
    return CL_Input::pointers[0]->get_axis(axis)->get_pos();
  }

  float
  MouseAxis::get_angle ()
  {
    return angle;
  }
  
  void
  MouseAxis::update(float)
  {
  }
}

/* EOF */
