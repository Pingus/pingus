//  $Id$
// 
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_VECTOR2_HPP
#define HEADER_VECTOR2_HPP

/** */
class Vector2i
{
public:
  int x;
  int y;

  Vector2i()
    : x(0), y(0) {}

  Vector2i(int x_, int y_) 
    : x(x_), y(y_) {}

  Vector2i operator- () const;

  Vector2i operator+ (const Vector2i& add) const;
  Vector2i operator- (const Vector2i& sub) const;
  Vector2i operator* (int  mul) const;

  Vector2i& operator+= (const Vector2i& add);
  Vector2i& operator-= (const Vector2i& sub);
  Vector2i& operator*= (int mul);
};

#endif

/* EOF */
