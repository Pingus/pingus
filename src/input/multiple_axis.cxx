//  $Id: multiple_axis.cxx,v 1.1 2002/07/10 11:21:53 torangan Exp $
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

#include "multiple_axis.hxx"

namespace Input
{
  MultipleAxis::MultipleAxis(float angle_, const std::vector<Axis*>& axes_) : 
                             angle(angle_), axes(axes_)
  {
    if (angle < 0)
      angle = (static_cast<int>(angle) % 360) + 360;
    else if (angle > 360)
      angle = static_cast<int>(angle) % 360;
  }

  float
  MultipleAxis::get_pos ()
  {
    for (std::vector<Axis*>::const_iterator it = axes.begin(); it != axes.end(); it++)
      {
        const float & temp = (*it)->get_pos();
	if (temp)
	  return temp;
      }

    return 0;
  }

  float
  MultipleAxis::get_angle ()
  {
    return angle;
  }
  
  void
  MultipleAxis::update(float delta)
  {
    for (std::vector<Axis*>::const_iterator it = axes.begin(); it != axes.end(); it++)
      (*it)->update(delta);
  }
}

/* EOF */
