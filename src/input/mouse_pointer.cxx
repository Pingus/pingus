//  $Id: mouse_pointer.cxx,v 1.1 2002/07/04 15:51:29 torangan Exp $
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

//#include <ClanLib/Core/Math/cl_vector.h>
#include <ClanLib/Display/Input/input.h>
#include <ClanLib/Display/Input/inputdevice.h>
#include <ClanLib/Display/Input/inputcursor.h>
#include "mouse_pointer.hxx"

namespace Input
{

  //MousePointer::MousePointer(float x_angle_, float y_angle_) : x_angle(x_angle_), y_angle(y_angle_) { }

  float
  MousePointer::get_x_pos ()
  {
    return CL_Input::pointers[0]->get_cursor(1)->get_x();
  }

  float
  MousePointer::get_y_pos ()
  {
    return CL_Input::pointers[0]->get_cursor(1)->get_y();
  }

  void
  MousePointer::update(float /*delta*/)
  {
/*
    CL_Vector pos_delta;
    float     x_delta(0), y_delta(0);

    // Axis 1
    pos_delta.x = CL_Input::pointers[0]->get_axis(0)->get_pos();
    pos_delta.y = 0;
    pos_delta.z = 0;
    
    pos_delta.rotate(x_angle, CL_Vector(0, 0, 1));
    
    x_delta += pos_delta.x;
    y_delta += pos_delta.y;

    // Axis 2
    pos_delta.x = CL_Input::pointers[0]->get_axis(1)->get_pos();
    pos_delta.y = 0;
    pos_delta.z = 0;
    
    pos_delta.rotate(y_angle, CL_Vector(0, 0, 1));
    
    x_delta += pos_delta.x;
    y_delta += pos_delta.y;
   

    x_pos += x_delta * delta;
    y_pos += y_delta * delta;
*/
  }

}

/* EOF */
