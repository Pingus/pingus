//  $Id: axis_scroller.hpp 3059 2007-09-02 03:48:52Z grumbel $
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

#ifndef HEADER_PINGUS_INPUT_AXIS_SCROLLER_HXX
#define HEADER_PINGUS_INPUT_AXIS_SCROLLER_HXX

#include <vector>
#include "../scroller.hpp"

namespace Input {

class Axis;

namespace Scrollers {

/**
   @brief create a Scroller out of two or more axes

   XML definition: <axis-scroller speed="?"> <axis 1>...<axis N> </axis-scroller>

   This class requires at least two axes whereby it's enforced that the first
   two have different angles.
*/
class AxisScroller : public Scroller {
private:
  const std::vector<Axis*> axes;

  const float speed;
  float x_delta;
  float y_delta;

public:
  AxisScroller(const std::vector<Axis*>& axes_, float speed_);
  ~AxisScroller();

  const float& get_x_delta() const;
  const float& get_y_delta() const;

  void  get_delta(float& x, float& y) const;

  void  update(float delta);

private:
  AxisScroller(const AxisScroller&);
  AxisScroller& operator= (const AxisScroller&);
};

} // namespace Scroller
} // namespace Input

#endif

/* EOF */
