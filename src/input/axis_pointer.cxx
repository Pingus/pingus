//  $Id: axis_pointer.cxx,v 1.1 2002/07/04 15:14:42 torangan Exp $
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

#include <ClanLib/Core/Math/cl_vector.h>
#include "axis.hxx"
#include "axis_pointer.hxx"

namespace Input
{
  AxisPointer::AxisPointer(Axis* axis1, Axis* axis2) : axes(2)
  {
    assert(axis1 && axis2);
    axes[0] = axis1;
    axes[1] = axis2;
  }

  AxisPointer::AxisPointer(std::vector<Axis*> axes_) : axes(axes_)
  {
    assert(axes.size() >= 2);
  }

  float
  AxisPointer::get_x_pos ()
  {
    return x_pos;
  }

  float
  AxisPointer::get_y_pos ()
  {
    return y_pos;
  }

  void
  AxisPointer::update(float delta)
  {
    CL_Vector pos_delta;
    float     x_delta(0), y_delta(0);
    
    for (std::vector<Axis*>::iterator it = axes.begin(); it != axes.end(); it++)
      {
        pos_delta.x = (*it)->get_pos();
        pos_delta.y = 0;
        pos_delta.z = 0;
	
        pos_delta.rotate((*it)->get_angle(), CL_Vector(0, 0, 1));
      
        x_delta += pos_delta.x;
        y_delta += pos_delta.y;
      } 
    
    x_pos += x_delta * delta;
    y_pos += y_delta * delta;
  }

}

/* EOF */
