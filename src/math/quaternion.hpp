//  $Id$
// 
//  Windstille - A Jump'n Shoot Game
//  Copyright (C) 2005 Matthias Braun <matze@braunis.de>
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

#ifndef __QUATERNION_HPP__
#define __QUATERNION_HPP__

#include "matrix.hpp"

class Quaternion
{
public:
  float w;
  float x;
  float y;
  float z;

  Quaternion()
    : w(0), x(0), y(0), z(0)
  {}

  Quaternion(float w, float x, float y, float z)
    : w(w), x(x), y(y), z(z)
  {}

  float magnitude() const;
  void normalize();

  const Quaternion& operator*= (const Quaternion& o)
  {
    *this = *this * o;
    return *this;
  }

  Quaternion operator* (const Quaternion& o) const
  {
    return Quaternion(
      w*o.w - x*o.x - y*o.y - z*o.z,
      w*o.x + x*o.w + y*o.z - z*o.y,
      w*o.y + y*o.w + z*o.x - x*o.z,
      w*o.z + z*o.w + x*o.y - y*o.x);
  }

  Quaternion operator- (const Quaternion& o) const
  {
    return Quaternion(w-o.w, x-o.x, y-o.y, z-o.z);
  }

  Quaternion operator+ (const Quaternion& o) const
  {
    return Quaternion(w+o.w, x+o.x, y+o.y, z+o.z);
  }

  Quaternion operator* (float s) const
  {
    return Quaternion(w*s, x*s, y*s, z*s);
  }

  float dot_product(const Quaternion& o) const
  {
    return x*o.x + y*o.y + z*o.z + w*o.w;
  }

  Matrix to_matrix() const;
  /**
   * spherical linear interpolation
   * Returns this quaternion rotation added with t* the way from this quaternion
   * to the o quaternion (so t should be between 0 and 1 usually)
   */
  Quaternion slerp(const Quaternion& o, float t) const;
};

#endif

/* EOF */
