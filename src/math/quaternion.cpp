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
#include <config.h>

#include "quaternion.hpp"

#include <math.h>

float
Quaternion::magnitude() const
{
  return sqrt(w*w + x*x + y*y + z*z);
}

void
Quaternion::normalize()
{
  float mag = magnitude();
  w /= mag;
  x /= mag;
  y /= mag;
  z /= mag;
}

Matrix
Quaternion::to_matrix() const
{
  Matrix r;
  r.matrix[0]  = 1.0f - 2.0f * (y*y + z*z);
  r.matrix[4]  =        2.0f * (x*y - z*w);
  r.matrix[8]  =        2.0f * (x*z + y*w);
  r.matrix[12] = 0.0f;

  r.matrix[1]  =        2.0f * (x*y + z*w);
  r.matrix[5]  = 1.0f - 2.0f * (x*x + z*z);
  r.matrix[9]  =        2.0f * (y*z - x*w);
  r.matrix[13] = 0.0f;
  
  r.matrix[2]  =        2.0f * (x*z - y*w);
  r.matrix[6]  =        2.0f * (y*z + x*w);
  r.matrix[10] = 1.0f - 2.0f * (x*x + y*y);
  r.matrix[14] = 0.0f;

  r.matrix[3]  = 0.0f;
  r.matrix[7]  = 0.0f;
  r.matrix[11] = 0.0f;
  r.matrix[15] = 1.0f;

  return r;
}

static float clamp(float val, float min, float max)
{
  if(val < min)
    val = min;
  else if(val > max)
    val = max;

  return val;
}

Quaternion
Quaternion::slerp(const Quaternion& o, float t) const
{
  /** Matze: I don't understand this code :-/ It's from
   * http://number-none.com/product/Understanding%20Slerp,%20Then%20Not%20Using%20It/
   * Though the article recommends not to use slerp I see no code for the other
   * methods so I'll use slerp anyway
   */
  float dot = dot_product(o);

  const float DOT_THRESHOLD = 0.995;
  if(dot > DOT_THRESHOLD) {
    // quaternions are too close, lineary interpolate them
    Quaternion result = *this + (o - *this)*t;
    result.normalize();
    return result;
  }
  
  dot = clamp(dot, -1 ,1); // robustness
  float theta_O = acos(dot);
  float theta = theta_O * t;

  Quaternion v2 = o - (*this * dot);
  v2.normalize();

  return (*this * cos(theta)) + (v2 * sin(theta));
}

/* EOF */
