//  $Id$
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
#include "../../debug.hpp"
#include "../../math.hpp"
#include "../axis.hpp"
#include "axis_scroller.hpp"

namespace Input {
namespace Scrollers {

AxisScroller::AxisScroller(const std::vector<Axis*>& axes_, float speed_)
  : axes(axes_),
    speed(speed_),
    x_delta(0),
    y_delta(0)
{
  assert(axes.size() > 1);
  assert(axes[0]->get_angle() != axes[1]->get_angle());
}

AxisScroller::~AxisScroller()
{
  for (unsigned int i=0; i < axes.size(); ++i)
    delete axes[i];
}

const float&
AxisScroller::get_x_delta() const
{
  return x_delta;
}

const float&
AxisScroller::get_y_delta() const
{
  return y_delta;
}

void
AxisScroller::get_delta(float& x, float& y) const
{
  x = x_delta;
  y = y_delta;
}

void
AxisScroller::update(float delta)
{
  x_delta = 0;
  y_delta = 0;

  for (std::vector<Axis*>::const_iterator it = axes.begin(); it != axes.end(); it++)
    {
      (*it)->update(delta);

      x_delta += (float)cos((*it)->get_angle() * Math::pi / 180) * speed * delta * (*it)->get_pos();
      y_delta += (float)sin((*it)->get_angle() * Math::pi / 180) * speed * delta * (*it)->get_pos();
    }

}

} // namespace Scrollers
} // namespace Input

/* EOF */

