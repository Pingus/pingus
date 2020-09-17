// Pingus - A free Lemmings clone
// Copyright (C) 2000 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_PINGUS_MATH_VECTOR3F_HPP
#define HEADER_PINGUS_MATH_VECTOR3F_HPP

#include "math/vector2f.hpp"

class Vector3f
{
private:
  float m_x;
  float m_y;
  //  float z;

public:
  explicit Vector3f (float x_=0, float y_=0 /*, float z_=0*/) : m_x(x_), m_y(y_)/*, z(z_)*/ {}

  Vector3f (const Vector3f& old) : m_x(old.m_x), m_y(old.m_y) {}
  Vector3f (const Vector2f& old) : m_x(old.x()), m_y(old.y()) {}
  Vector3f (const Vector2i& old) : m_x(float(old.x())), m_y(float(old.y())) {}
  Vector3f& operator= (const Vector3f& old)
  {
    if (this != &old)
    {
      m_x = old.m_x;
      m_y = old.m_y;
    }

    return *this;
  }

  float x() const { return m_x; }
  float y() const { return m_y; }

  Vector3f operator- () const
  {
    return Vector3f(-m_x, -m_y);
  }

  Vector3f operator+ (const Vector3f& add) const
  {
    return Vector3f(m_x + add.m_x, m_y + add.m_y);
  }
  Vector3f operator- (const Vector3f& sub) const
  {
    return Vector3f(m_x - sub.m_x, m_y - sub.m_y);
  }
  Vector3f operator* (float mul) const
  {
    return Vector3f(mul * m_x, mul * m_y);
  }

  bool operator==(Vector3f const& rhs) const
  {
    return (m_x == rhs.m_x &&
            m_y == rhs.m_y);
  }

  Vector3f& operator+= (const Vector3f& add)
  {
    m_x += add.m_x;
    m_y += add.m_y;
    return *this;
  }

  Vector3f& operator-= (const Vector3f& sub)
  {
    m_x -= sub.m_x;
    m_y -= sub.m_y;
    return *this;
  }

#if 0
  Vector3f& operator*= (float mul)
  {
    m_x *= mul;
    m_y *= mul;
    return *this;
  }
#endif

  void normalize ()
  {
    float f = Math::sqrt(m_x * m_x + m_y * m_y);

    if (f != 0.0f)
    {
      m_x /= f;
      m_y /= f;
    }
  }

  float length() const
  {
    return Math::sqrt(m_x * m_x + m_y * m_y);
  }

  static float distance2d(const Vector3f& a, const Vector3f& b)
  {
    float m_x = b.m_x - a.m_x;
    float m_y = b.m_y - a.m_y;

    return Math::abs(Math::sqrt((m_x * m_x) + (m_y * m_y)));
  }

  /** Calculate a position between a and b, relative to the value of
      \a perc (perc == 0 -> a, perc == 1.0 -> b) */
  static Vector3f interpolate(const Vector3f& a, const Vector3f& b, float perc)
  {
    Vector3f c = b - a;
    return a + (c * perc);
  }
};

std::ostream& operator<< (std::ostream& os, const Vector3f& v);

#endif

/* EOF */
