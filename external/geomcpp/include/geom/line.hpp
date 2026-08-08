// geomcpp - Basic collection of point, size and rect classes
// Copyright (C) 2020 Ingo Ruhnke <grumbel@gmail.com>
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef HEADER_GEOM_LINE_HPP
#define HEADER_GEOM_LINE_HPP

#include "point.hpp"

namespace geom {

template<typename T>
class tline
{
public:
  tline() : p1(), p2() {}

  tline(tpoint<T> const& p1_,
        tpoint<T> const& p2_)
    : p1(p1_), p2(p2_)
  {}

  float length() const {
    return geom::distance(p1, p2);
  }

  /** Calculate if two lines intersec */
  bool intersect(tline<T> const& line_b) const
  {
    float ua;
    float ub;
    return intersect(line_b, ua, ub);
  }

  /** Calculate if and where two lines intersect */
  bool intersect(tline<T> const& rhs, float& ua, float& ub) const
  {
    float const& x1 = p1.x();
    float const& y1 = p1.y();

    float const& x2 = p2.x();
    float const& y2 = p2.y();

    float const& x3 = rhs.p1.x();
    float const& y3 = rhs.p1.y();

    float const& x4 = rhs.p2.x();
    float const& y4 = rhs.p2.y();

    float quotient = ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));

    if (quotient == 0.0f) {
      return false; // FIXME
    } else {
      ua = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / quotient;
      ub = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / quotient;

      return (ua >= 0.0f && ua <= 1.0f &&
              ub >= 0.0f && ub <= 1.0f);
    }
  }

  /** Calculate if and where two lines intersect */
  bool intersect(const tline<T>& line, tpoint<T>& colpos) const
  {
    float ua;
    float ub;
    if (intersect(line, ua, ub))
    {
      colpos = tpoint<T>(p1.as_vec() + ((p2.as_vec() - p1.as_vec()) * ua));
      return true;
    }
    else
    {
      return false;
    }
  }

  /** Calculate the minimal distance between this line and the point p */
  float distance(tpoint<T> const& p3) const
  {
    float const& x1 = p1.x();
    float const& y1 = p1.y();

    float const& x2 = p2.x();
    float const& y2 = p2.y();

    float const& x3 = p3.x();
    float const& y3 = p3.y();

    float u =
      ((x3 - x1) * (x2 - x1) + (y3 - y1) * (y2 - y1)) /
      (length() * length());

    if (u < 0.0f) {
      return glm::distance(p3.as_vec(), p1.as_vec());
    } else if (u > 1.0f) {
      return glm::distance(p3.as_vec(), p2.as_vec());
    } else /* (u >= 0.0f && u <= 1.0f) */ {
      glm::vec2 p4(x1 + u * (x2 - x1),
                   y1 + u * (y2 - y1));

      return glm::distance(p3.as_vec(), p4);
    }
  }

public:
  tpoint<T> p1;
  tpoint<T> p2;
};

using iline = tline<int>;
using fline = tline<float>;

} // namespace geom

#endif

/* EOF */
