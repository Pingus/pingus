//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_PINGUS_MATH_VECTOR3F_HPP
#define HEADER_PINGUS_MATH_VECTOR3F_HPP

#include <iosfwd>

class Vector2f;
class Vector2i;

class Vector3f
{
public:
  float x;
  float y;
  float z;

public:
  explicit Vector3f (float x_=0, float y_=0, float z_=0);

  Vector3f (const Vector3f& old);
  Vector3f (const Vector2f& old);
  Vector3f (const Vector2i& old);
  Vector3f& operator= (const Vector3f& old);

  Vector3f operator- () const;

  Vector3f operator+ (const Vector3f& add) const;
  Vector3f operator- (const Vector3f& sub) const;
  Vector3f operator* (       float  mul) const;

  Vector3f& operator+= (const Vector3f& add);
  Vector3f& operator-= (const Vector3f& sub);
  Vector3f& operator*= (      float   mul);

  void normalize ();

  float length() const;

  Vector3f rotate (float angle, const Vector3f& pos) const;

  static float distance(const Vector3f& a, const Vector3f& b);
  static float distance2d(const Vector3f& a, const Vector3f& b);

  /** Calculate a position between a and b, relative to the value of
      \a perc (perc == 0 -> a, perc == 1.0 -> b) */
  static Vector3f interpolate(const Vector3f& a, const Vector3f& b, float perc);
};

std::ostream& operator<< (std::ostream& os, const Vector3f& v);

#endif

/* EOF */
