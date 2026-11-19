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

#ifndef HEADER_GEOM_QUAD_HPP
#define HEADER_GEOM_QUAD_HPP

#include <math.h>

// Avoid <glm/ext.hpp>: GLM 1.0+ packing.inl pulls <endian.h> (missing on MinGW).
#include <glm/glm.hpp>

#include "rect.hpp"

namespace geom {

template<typename T>
class tquad
{
public:
  tquad() :
    p1(),
    p2(),
    p3(),
    p4()
  {}

  tquad(trect<T> const& rect) :
    p1(rect.left(), rect.top()),
    p2(rect.right(), rect.top()),
    p3(rect.right(), rect.bottom()),
    p4(rect.left(), rect.bottom())
  {}

  tquad(T x1, T y1,
        T x2, T y2) :
    p1(x1, y1),
    p2(x2, y1),
    p3(x2, y2),
    p4(x1, y2)
  {}

  tquad(tpoint<T> const& p1_,
        tpoint<T> const& p2_,
        tpoint<T> const& p3_,
        tpoint<T> const& p4_) :
    p1(p1_),
    p2(p2_),
    p3(p3_),
    p4(p4_)
  {}

  geom::trect<T> get_bounding_box() const
  {
    return geom::trect<T>(std::min(std::min(std::min(p1.x(), p2.x()), p3.x()), p4.x()),
                          std::min(std::min(std::min(p1.y(), p2.y()), p3.y()), p4.y()),
                          std::max(std::max(std::max(p1.x(), p2.x()), p3.x()), p4.x()),
                          std::max(std::max(std::max(p1.y(), p2.y()), p3.y()), p4.y()));
  }

  /** Rotate by \a rad radians around the center of the quad */
  void rotate(float rad)
  {
    float const cx = static_cast<float>(p1.x() + p2.x() + p3.x() + p4.x()) / 4.0f;
    float const cy = static_cast<float>(p1.y() + p2.y() + p3.y() + p4.y()) / 4.0f;
    float const c = cosf(rad);
    float const s = sinf(rad);

    auto const rot = [&](tpoint<T> const& p) {
      float const dx = static_cast<float>(p.x()) - cx;
      float const dy = static_cast<float>(p.y()) - cy;
      return tpoint<T>(static_cast<T>(cx + c * dx - s * dy),
                       static_cast<T>(cy + s * dx + c * dy));
    };

    p1 = rot(p1);
    p2 = rot(p2);
    p3 = rot(p3);
    p4 = rot(p4);
  }

public:
  tpoint<T> p1;
  tpoint<T> p2;
  tpoint<T> p3;
  tpoint<T> p4;
};

using iquad = tquad<int>;
using fquad = tquad<float>;

} // namespace geom

#endif

/* EOF */
