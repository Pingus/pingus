//  $Id: multiple_axis.cpp 3059 2007-09-02 03:48:52Z grumbel $
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

#include "multiple_axis.hpp"

namespace Input {
namespace Axes {

MultipleAxis::MultipleAxis (const std::vector<Axis*>& axes_) : axes(axes_),
                                                               pos(0), angle(0)
{
}

MultipleAxis::~MultipleAxis ()
{
  for (std::vector<Axis*>::const_iterator it = axes.begin(); it != axes.end(); it++)
    delete *it;
}

const float&
MultipleAxis::get_pos () const
{
  return pos;
}

const float&
MultipleAxis::get_angle () const
{
  return angle;
}

void
MultipleAxis::update (float delta)
{
  for (std::vector<Axis*>::const_iterator it = axes.begin(); it != axes.end(); it++)
    {
      (*it)->update(delta);
      const float & temp = (*it)->get_pos();
      if (temp)
        {
          pos   = temp;
          angle = (*it)->get_angle();
        }

    }
}

} // namespace Axes
} // namespace Input

/* EOF */
