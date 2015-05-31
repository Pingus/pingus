// Pingus - A free Lemmings clone
// Copyright (C) 2005 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_MATH_COLOR_HPP
#define HEADER_PINGUS_MATH_COLOR_HPP

#include <SDL.h>

class Color
{
public:
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;

  Color()
    : r(0), g(0), b(0), a(255)
  {}

  Color(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_ = 255)
    : r(r_), g(g_), b(b_), a(a_)
  {}

  bool operator==(const Color& rhs) const {
    return
      r == rhs.r &&
      g == rhs.g &&
      b == rhs.b &&
      a == rhs.a;
  }
};

#endif

/* EOF */
