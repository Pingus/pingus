//  $Id: axis_pointer.cxx,v 1.5 2003/10/20 19:28:55 grumbel Exp $
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

#include <math.h>
#include <assert.h>
#include "../axis.hpp"
#include "axis_pointer.hpp"

namespace Input {
namespace Pointers {

AxisPointer::AxisPointer(float speed_, const std::vector<Axis*>& axes_) : speed(speed_), axes(axes_)
{
  assert(axes.size() >= 2);
  assert(axes[0]->get_angle() != axes[1]->get_angle());
}

AxisPointer::~AxisPointer()
{
  for (std::vector<Axis*>::const_iterator it = axes.begin(); it != axes.end(); it++)
    delete *it;
}

const float&
AxisPointer::get_x_pos() const
{
  return x_pos;
}

const float&
AxisPointer::get_y_pos() const
{
  return y_pos;
}

void
AxisPointer::set_pos(float new_x, float new_y)
{
  x_pos = new_x;
  y_pos = new_y;
}

void
AxisPointer::update(float delta)
{
  for (std::vector<Axis*>::const_iterator it = axes.begin(); it != axes.end(); it++)
    {
      (*it)->update(delta);

      x_pos += (float)cos((*it)->get_angle() * 3.14159265 / 180) * speed * delta * (*it)->get_pos();
      y_pos += (float)sin((*it)->get_angle() * 3.14159265 / 180) * speed * delta * (*it)->get_pos();
    }
}

} // namespace Axes
} // namespace Input

/* EOF */
