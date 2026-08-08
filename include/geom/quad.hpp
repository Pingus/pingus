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

#include <glm/ext.hpp>
#include <glm/gtx/rotate_vector.hpp>

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

  tquad(const geom::frect& rect) :
    p1(rect.left(), rect.top()),
    p2(rect.right(), rect.top()),
    p3(rect.right(), rect.bottom()),
    p4(rect.left(), rect.bottom())
  {}

  tquad(float x1, float y1,
       float x2, float y2) :
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

  void rotate(float rad)
  {
    glm::vec2 center(static_cast<float>(p1.x() + p2.x() + p3.x() + p4.x()) / 4.0f,
                     static_cast<float>(p1.y() + p2.y() + p3.y() + p4.y()) / 4.0f);

    p1 = glm::tvec2<T>(center + glm::rotate(glm::vec2(p1.as_vec()) - center, rad));
    p2 = glm::tvec2<T>(center + glm::rotate(glm::vec2(p2.as_vec()) - center, rad));
    p3 = glm::tvec2<T>(center + glm::rotate(glm::vec2(p3.as_vec()) - center, rad));
    p4 = glm::tvec2<T>(center + glm::rotate(glm::vec2(p4.as_vec()) - center, rad));
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
