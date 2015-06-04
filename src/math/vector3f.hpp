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
public:
  float x;
  float y;
  float z;

public:
  explicit Vector3f (float x_=0, float y_=0, float z_=0) : x(x_), y(y_), z(z_) {}

  Vector3f (const Vector3f& old) : x(old.x), y(old.y), z(old.z) {}
  Vector3f (const Vector2f& old) : x(old.x), y(old.y), z(0) {}
  Vector3f (const Vector2i& old) : x(float(old.x)), y(float(old.y)), z(0) {}
  Vector3f& operator= (const Vector3f& old)
  {
    if (this != &old)
    {
      x = old.x;
      y = old.y;
      z = old.z;
    }

    return *this;
  }

  Vector3f operator- () const
  {
    return Vector3f(-x, -y, -z);
  }

  Vector3f operator+ (const Vector3f& add) const
  {
    return Vector3f(x + add.x, y + add.y, z + add.z);
  }
  Vector3f operator- (const Vector3f& sub) const
  {
    return Vector3f(x - sub.x, y - sub.y, z - sub.z);
  }
  Vector3f operator* (float mul) const
  {
    return Vector3f(mul * x, mul * y, mul * z);
  }

  bool operator==(Vector3f const& rhs) const
  {
    return (x == rhs.x &&
            y == rhs.y &&
            z == rhs.z);
  }

  Vector3f& operator+= (const Vector3f& add)
  {
    x += add.x;
    y += add.y;
    z += add.z;
    return *this;
  }
  Vector3f& operator-= (const Vector3f& sub)
  {
    x -= sub.x;
    y -= sub.y;
    z -= sub.z;
    return *this;
  }
  Vector3f& operator*= (float mul)
  {
    x *= mul;
    y *= mul;
    z *= mul;
    return *this;
  }

  void normalize ()
  {
    float f = Math::sqrt(x * x + y * y + z * z);

    if (f != 0.0f)
    {
      x /= f;
      y /= f;
      z /= f;
    }
  }

  float length() const
  {
    return Math::sqrt(x * x + y * y + z * z);
  }

  Vector3f rotate (float angle, const Vector3f& pos) const;

  static float distance(const Vector3f& a, const Vector3f& b)
  {
    float x = b.x - a.x;
    float y = b.y - a.y;
    float z = b.z - a.z;

    return Math::abs(Math::sqrt((x * x) + (y * y) + (z * z)));
  }
  static float distance2d(const Vector3f& a, const Vector3f& b)
  {
    float x = b.x - a.x;
    float y = b.y - a.y;

    return Math::abs(Math::sqrt((x * x) + (y * y)));
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
