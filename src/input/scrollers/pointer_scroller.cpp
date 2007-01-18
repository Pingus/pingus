//  $Id: pointer_scroller.cxx,v 1.5 2003/10/20 13:33:44 grumbel Exp $
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

#include "../button.hpp"
#include "../pointer.hpp"
#include "pointer_scroller.hpp"

namespace Input {
namespace Scrollers {

PointerScroller::PointerScroller (Pointer* pointer_, Button* modifier_)
  : pointer(pointer_),
    modifier(modifier_),
    x_delta(0),
    y_delta(0),
    x_pos(-1),
    y_pos(-1)
{
}

PointerScroller::~PointerScroller ()
{
  delete pointer;
  delete modifier;
}

const float&
PointerScroller::get_x_delta () const
{
  return x_delta;
}

const float&
PointerScroller::get_y_delta () const
{
  return y_delta;
}

void
PointerScroller::get_delta (float& x, float& y) const
{
  x = x_delta;
  y = y_delta;
}

void
PointerScroller::update (float delta)
{
  pointer ->update(delta);
  modifier->update(delta);

  if (modifier->is_pressed())
    {
      if (x_pos == -1)
        {
          x_pos = pointer->get_x_pos();
          y_pos = pointer->get_y_pos();
        }
      else
        {
          x_delta = pointer->get_x_pos() - x_pos;
          y_delta = pointer->get_y_pos() - y_pos;

          pointer->set_pos(x_pos, y_pos);
        }
    }
  else
    {
      x_pos = -1;
      y_pos = -1;
    }
}

} // namespace Axes
} // namespace Input

/* EOF */
