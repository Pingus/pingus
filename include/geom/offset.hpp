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

#ifndef HEADER_GEOMCPP_OFFSET_HPP
#define HEADER_GEOMCPP_OFFSET_HPP

#include <math.h>
#include <glm/glm.hpp>

#include "fwd.hpp"
#include "origin.hpp"
#include "size.hpp"

namespace geom {

template<typename T>
class toffset
{
public:
  constexpr toffset() : m_x(), m_y() {}
  constexpr toffset(T x, T y) :
    m_x(x), m_y(y) {}
  constexpr toffset(glm::tvec2<T> const& v) :
    m_x(v.x), m_y(v.y) {}
  constexpr toffset(geom::tpoint<T> const& v) :
    m_x(v.x()), m_y(v.y()) {}
  constexpr toffset(toffset const& p) = default;

  template<typename From>
  constexpr explicit toffset(toffset<From> const& offset) :
    m_x(static_cast<T>(offset.x())),
    m_y(static_cast<T>(offset.y()))
  {}

  constexpr T x() const { return m_x; }
  constexpr T y() const { return m_y; }

  constexpr bool operator==(const toffset& other) const {
    return (m_x == other.m_x &&
            m_y == other.m_y);
  }

  constexpr bool operator!=(const toffset& other) const {
    return !(*this == other);
  }

  toffset<T> operator-() const {
    return toffset<T>(-m_x, -m_y);
  }

  toffset<T>& operator+=(toffset<T> const& other) {
    m_x += other.x();
    m_y += other.y();
    return *this;
  }

  toffset<T>& operator-=(toffset<T> const& other) {
    m_x -= other.x();
    m_y -= other.y();
    return *this;
  }

  toffset<T>& operator*=(T s) {
    m_x *= s;
    m_y *= s;
    return *this;
  }

  toffset<T>& operator/=(T s) {
    m_x /= s;
    m_y /= s;
    return *this;
  }

  glm::vec<2, T> as_vec() const { return glm::vec<2, T>(m_x, m_y); }

private:
  T m_x;
  T m_y;
};

template<typename T> inline
toffset<T> transpose(toffset<T> const offset)
{
  return toffset<T>(offset.y(), offset.x());
}

template<typename T> inline
toffset<T> operator+(toffset<T> const& lhs, toffset<T> const& rhs) {
  return toffset<T>(lhs.x() + rhs.x(),
                    lhs.y() + rhs.y());
}

template<typename T> inline
toffset<T> operator-(toffset<T> const& lhs, toffset<T> const& rhs) {
  return toffset<T>(lhs.x() - rhs.x(),
                    lhs.y() - rhs.y());
}

template<typename T> inline
toffset<T> operator*(T s, toffset<T> const& offset) {
  return toffset<T>(s * offset.x(),
                    s * offset.y());
}

template<typename T> inline
toffset<T> operator*(toffset<T> const& offset, T s) {
  return toffset<T>(offset.x() * s,
                    offset.y() * s);
}

template<typename T> inline
toffset<T> operator/(toffset<T> const& offset, T s) {
  return toffset<T>(offset.x() / s,
                    offset.y() / s);
}

template<typename T> inline
toffset<T> anchor_offset(tsize<T> const& size, origin origin)
{
  switch(origin)
  {
    case origin::TOP_LEFT:
      return toffset<T>(0, 0);

    case origin::TOP_CENTER:
      return toffset<T>(-size.width() / 2, 0);

    case origin::TOP_RIGHT:
      return toffset<T>(-size.width(), 0);

    case origin::CENTER_LEFT:
      return toffset<T>(0, -size.height() / 2);

    case origin::CENTER:
      return toffset<T>(-size.width() / 2, -size.height() / 2);

    case origin::CENTER_RIGHT:
      return toffset<T>(-size.width(), -size.height() / 2);

    case origin::BOTTOM_LEFT:
      return toffset<T>(0, -size.height());

    case origin::BOTTOM_CENTER:
      return toffset<T>(-size.width() / 2, -size.height());

    case origin::BOTTOM_RIGHT:
      return toffset<T>(-size.width(), -size.height());

    default:
      throw std::invalid_argument("invalid value for origin");
  }
}

using ioffset = toffset<int>;
using foffset = toffset<float>;

} // namespace geom

#endif

/* EOF */
