//  $Id: vector.hxx,v 1.4 2003/04/19 10:23:17 torangan Exp $
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

#ifndef HEADER_PINGUS_VECTOR_HXX
#define HEADER_PINGUS_VECTOR_HXX

#include <iosfwd>
#include "pingus.hxx"

class Vector
{
public:
  float x;
  float y;
  float z;

public:
  explicit Vector (float x_=0, float y_=0, float z_=0);

  Vector (const Vector& old);
  Vector& operator= (const Vector& old);

  Vector operator- () const;

  Vector operator+ (const Vector& add) const;
  Vector operator- (const Vector& sub) const;
  Vector operator* (       float  mul) const;

  Vector& operator+= (const Vector& add);
  Vector& operator-= (const Vector& sub);
  Vector& operator*= (      float   mul);

  void normalize ();

  float length() const;

  Vector rotate (float angle, const Vector& pos) const;

  static float distance(const Vector& a, const Vector& b);
  static float distance2d(const Vector& a, const Vector& b);

  /** Calculate a position between a and b, relative to the value of
      \a perc (perc == 0 -> a, perc == 1.0 -> b) */
  static Vector interpolate(const Vector& a, const Vector& b, float perc);

  friend std::ostream& operator<< (std::ostream& os, const Vector& v);
};

std::ostream& operator<< (std::ostream& os, const Vector& v);

#endif

/* EOF */
