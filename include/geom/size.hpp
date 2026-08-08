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

#ifndef HEADER_GEOMCPP_SIZE_HPP
#define HEADER_GEOMCPP_SIZE_HPP

#include <stdexcept>

#include <glm/glm.hpp>

#include "point.hpp"
#include "origin.hpp"

namespace geom {

template<typename T>
class tsize
{
public:
  constexpr tsize() : m_width(), m_height() {}
  constexpr tsize(T width, T height) : m_width(width), m_height(height) {}
  explicit constexpr tsize(glm::vec<2, T> const& v) : m_width(v.x), m_height(v.y) {}
  constexpr tsize(tsize<T> const& size) = default;

  template<typename From>
  constexpr explicit tsize(tsize<From> const& size) :
    m_width(static_cast<T>(size.width())),
    m_height(static_cast<T>(size.height()))
  {}

  constexpr T width() const { return m_width; }
  constexpr T height() const { return m_height; }

  glm::tvec2<T> as_vec() const { return glm::tvec2<T>(m_width, m_height); }

  constexpr bool operator==(const tsize<T>& other) const {
    return (m_width == other.m_width &&
            m_height == other.m_height);
  }

  constexpr bool operator!=(const tsize<T>& other) const {
    return !(*this == other);
  }

  bool is_valid() const {
    return m_width >= 0 && m_height >= 0;
  }

  bool is_null() const {
    return m_width == 0 && m_height == 0;
  }

  bool is_empty() const {
    return m_width <= 0 || m_height <= 0;
  }

private:
  T m_width;
  T m_height;
};

template<typename T> inline
tsize<T> operator*(T s, tsize<T> const& size) {
  return tsize<T>(s * size.width(),
                  s * size.height());
}

template<typename T> inline
tsize<T> operator*(tsize<T> const& size, T s) {
  return s * size;
}

template<typename T> inline
tsize<T> operator/(tsize<T> const& size, T s) {
  return tsize<T>(size.width() / s,
                  size.height() / s);
}

template<typename T> inline
T area(tsize<T> const& size) {
  return size.width() * size.height();
}

template<typename T> inline
tsize<T> transpose(tsize<T> const& size)
{
  return tsize<T>(size.height(), size.width());
}

template<typename T> inline
float aspect_ratio(tsize<T> const& size)
{
  return static_cast<float>(size.width()) / static_cast<float>(size.height());
}

template<typename T> inline
bool contains(tsize<T> const& parent, tsize<T> const& child)
{
  return (parent.width() >= child.width() &&
          parent.height() >= child.height());
}

template<typename T> inline
bool contains(tsize<T> const& parent, tpoint<T> const& point)
{
  return (0 <= point.x() && point.x() < parent.width() &&
          0 <= point.y() && point.y() < parent.height());
}

template<typename T> inline
tsize<T> resize_to_fit(tsize<T> const& src, tsize<T> const& target)
{
  if (aspect_ratio(src) > aspect_ratio(target)) {
    return tsize<T>(target.width(),
                    src.height() * target.width() / src.width());
  } else {
    return tsize<T>(src.width() * target.height() / src.height(),
                    target.height());
  }
}

template<typename T> inline
tsize<T> grow_to_fit(tsize<T> const& src, tsize<T> const& target)
{
  if (contains(target, src)) {
    return resize_to_fit(src, target);
  } else {
    return src;
  }
}

template<typename T> inline
tsize<T> shrink_to_fit(tsize<T> const& src, tsize<T> const& target)
{
  if (contains(src, target)) {
    return resize_to_fit(src, target);
  } else {
    return src;
  }
}

using isize = tsize<int>;
using fsize = tsize<float>;

} // namespace geom

#endif

/* EOF */
