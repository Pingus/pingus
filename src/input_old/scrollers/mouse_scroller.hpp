
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

#ifndef HEADER_PINGUS_INPUT_MOUSE_SCROLLER_HXX
#define HEADER_PINGUS_INPUT_MOUSE_SCROLLER_HXX

#include "../scroller.hpp"

namespace Input {
namespace Scrollers {

/**
   @brief turns the mouse into a scroller

   XML definition: <mouse-scroller/>
*/
class MouseScroller : public Scroller {
private:
  float old_x;
  float old_y;
  float x_delta;
  float y_delta;

public:
  MouseScroller(int id);

  const float& get_x_delta() const;
  const float& get_y_delta() const;

  void  get_delta(float& x, float& y) const;

  void  update(float);

private:
  MouseScroller(const MouseScroller&);
  MouseScroller& operator= (const MouseScroller&);
};

} // namespace Scrollers
} // namespace Input

#endif

/* EOF */
