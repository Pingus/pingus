//  $Id: axis_scroller.cxx,v 1.1 2002/07/11 12:36:15 torangan Exp $
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

#include "axis.hxx"
#include "axis_scroller.hxx"

namespace Input {

  AxisScroller::AxisScroller (Axis* axis1_, Axis* axis2_, float speed_) : axis1(axis1_), axis2(axis2_), speed(speed_)
                                                  
  {
  }
  
  AxisScroller::~AxisScroller ()
  {
    delete axis1;
    delete axis2;
  }
  
  float
  AxisScroller::get_x_delta ()
  {
    return axis1->get_pos() * speed;
  }
  
  float
  AxisScroller::get_y_delta ()
  {
    return axis2->get_pos() * speed;
  }
  
  void
  AxisScroller::get_delta (float& x, float& y)
  {
    x = axis1->get_pos() * speed;
    y = axis2->get_pos() * speed;
  }
  
  void
  AxisScroller::update (float delta)
  {
    axis1->update(delta);
    axis2->update(delta);
  }
}

/* EOF */
