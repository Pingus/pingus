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

#include <math.h>
#include "math.hpp"

namespace Math {

float abs(float v)
{
  return ::fabs(v);
}

float sin(float a)
{
  return ::sinf(a);
}

float cos(float a)
{
  return ::cosf(a);
}

float sqrt(float a)
{
  return ::sqrt(a);
}

float mod(float x, float y)
{
  return ::fmodf(x, y);
}

float floor(float x)
{
  return ::floorf(x);
}

float atan2(float x, float y)
{
  return ::atan2(x, y);
}

} // namespace Math

/* EOF */
