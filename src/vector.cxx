//  $Id: vector.cxx,v 1.5 2003/10/18 23:17:27 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#include <math.h>
#include <iostream>
#include "vector.hxx"

Vector::Vector (float x_, float y_, float z_) : x(x_), y(y_), z(z_)
{
}

Vector::Vector (const Vector& old) : x(old.x), y(old.y), z(old.z)
{
}

Vector&
Vector::operator= (const Vector& old)
{
  if (this != &old)
    {
      x = old.x;
      y = old.y;
      z = old.z;
    }

  return *this;
}

Vector
Vector::operator- () const
{
  return Vector(-x, -y, -z);
}

Vector
Vector::operator+ (const Vector& add) const
{
  return Vector(x + add.x, y + add.y, z + add.z);
}

Vector
Vector::operator- (const Vector& sub) const
{
  return Vector(x - sub.x, y - sub.y, z - sub.z);
}

Vector
Vector::operator* (float mul) const
{
  return Vector(mul * x, mul * y, mul * z);
}

Vector&
Vector::operator+= (const Vector& add)
{
  x += add.x;
  y += add.y;
  z += add.z;
  return *this;
}

Vector&
Vector::operator-= (const Vector& sub)
{
  x -= sub.x;
  y -= sub.y;
  z -= sub.z;
  return *this;
}

Vector&
Vector::operator*= (float mul)
{
  x *= mul;
  y *= mul;
  z *= mul;
  return *this;
}

void
Vector::normalize ()
{
  float f = sqrt(x * x + y * y + z * z);

  if (f)
    {
      x /= f;
      y /= f;
      z /= f;
    }
}

float
Vector::length() const
{
  return sqrt(x * x + y * y + z * z);
}

Vector
Vector::rotate (float angle, const Vector& pos) const
{
  const float s = sin(angle);
  const float c = cos(angle);

  return Vector(  x * (pos.x * pos.x * (1-c) + c)
                + y * (pos.x * pos.y * (1-c) - pos.z *s)
		+ z * (pos.x * pos.z * (1-c) + pos.y *s),

		  x * (pos.y * pos.x * (1-c) + pos.z *s)
		+ y * (pos.y * pos.y * (1-c) + c)
		+ z * (pos.y * pos.z * (1-c) - pos.x *s),

		  x * (pos.x * pos.z * (1-c) - pos.y *s)
		+ y * (pos.y * pos.z * (1-c) + pos.x *s)
		+ z * (pos.z * pos.z * (1-c) + c)
	       );
}

float
Vector::distance(const Vector& a, const Vector& b)
{
  float x = b.x - a.x;
  float y = b.y - a.y;
  float z = b.z - a.z;

  return fabsf(sqrt((x * x) + (y * y) + (z * z)));
}

float
Vector::distance2d(const Vector& a, const Vector& b)
{
  float x = b.x - a.x;
  float y = b.y - a.y;

  return fabsf(sqrt((x * x) + (y * y)));
}

Vector
Vector::interpolate(const Vector& a, const Vector& b, float perc)
{
  Vector c = b - a;
  return a + (c * perc);
}

std::ostream& operator<< (std::ostream& os, const Pingus::Vector& v)
{
  return os << v.x << " " << v.y << " " << v.z;
}

/* EOF */
