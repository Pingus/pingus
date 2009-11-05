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

#include "multiple_scroller.hpp"

namespace Input {
namespace Scrollers {

MultipleScroller::MultipleScroller (const std::vector<Scroller*>& scrollers_) : scrollers(scrollers_)
{
}

MultipleScroller::~MultipleScroller ()
{
  for (std::vector<Scroller*>::const_iterator it = scrollers.begin(); it != scrollers.end(); it++)
    delete *it;
}

const float&
MultipleScroller::get_x_delta () const
{
  return x_pos;
}

const float&
MultipleScroller::get_y_delta () const
{
  return y_pos;
}

void
MultipleScroller::get_delta (float& x, float& y) const
{
  x = x_pos;
  y = y_pos;
}

void
MultipleScroller::update (float delta)
{
  bool found_delta = false;

  for (std::vector<Scroller*>::const_iterator it = scrollers.begin(); it != scrollers.end(); it++)
    {
      (*it)->update(delta);

      if (!found_delta && ((*it)->get_x_delta() || (*it)->get_y_delta()))
        {
          x_pos = (*it)->get_x_delta();
          y_pos = (*it)->get_y_delta();
          found_delta = true;
        }
    }
}

} // namespace Scroller
} // namespace Input

/* EOF */
