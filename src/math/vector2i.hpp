// Pingus - A free Lemmings clone
// Copyright (C) 2002 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_MATH_VECTOR2I_HPP
#define HEADER_PINGUS_MATH_VECTOR2I_HPP

class Vector2f;

class Vector2i
{
public:
  int x;
  int y;

  Vector2i()
    : x(0), y(0) {}

  explicit Vector2i(const Vector2f& v);

  Vector2i(int x_, int y_)
    : x(x_), y(y_) {}

  Vector2i operator+ (const Vector2i& add) const
  {
    return Vector2i(x + add.x, y + add.y);
  }

  Vector2i operator- () const
  {
    return Vector2i(-x, -y);
  }

  Vector2i operator- (const Vector2i& sub) const
  {
    return Vector2i(x - sub.x, y - sub.y);
  }

  Vector2i operator* (int  mul) const
  {
    return Vector2i(x * mul, y * mul);
  }

  Vector2i& operator+= (const Vector2i& add)
  {
    x += add.x;
    y += add.y;
    return *this;
  }

  Vector2i& operator-= (const Vector2i& sub)
  {
    x -= sub.x;
    y -= sub.y;
    return *this;
  }

  Vector2i& operator*= (int mul)
  {
    x *= mul;
    y *= mul;
    return *this;
  }

  bool operator== (const Vector2i& other) const
  {
    return (other.x == x && other.y == y);
  }

  bool operator!= (const Vector2i& other) const
  {
    return (other.x != x || other.y != y);
  }
};

std::ostream& operator<<(std::ostream& s, const Vector2i& v);

#endif

/* EOF */
