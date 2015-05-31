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

#include "math/math.hpp"

#include <math.h>

namespace Math {

float abs(float v)
{
  return ::fabsf(v);
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
  return ::sqrtf(a);
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
  return ::atan2f(x, y);
}

static char num2hex[] = "0123456789abcdef";

std::string float2string(float value)
{
  std::string str(2*sizeof(float), '0');

  for(size_t i = 0; i < sizeof(float); ++i)
  {
    char v = reinterpret_cast<char*>(&value)[i];
    str[2*i + 0] = num2hex[(v & 0xf0) >> 4];
    str[2*i + 1] = num2hex[v & 0x0f];
  }
  return str;
}

static char hex2int(char c)
{
  if (c >= '0' && c <= '9')
    return static_cast<char>(c - '0');
  else if (c >= 'a' && c <= 'f')
    return static_cast<char>(c - 'a' + 0xa);
  else
    return 0;
}

float string2float(const std::string& str)
{
  assert(str.size() == 2*sizeof(float));

  float value;
  for(size_t i = 0; i < sizeof(float); ++i)
  {
    char& v = reinterpret_cast<char*>(&value)[i];
    v = static_cast<char>((hex2int(str[2*i+0]) << 4) | hex2int(str[2*i+1]));
  }

  return value;
}

} // namespace Math

/* EOF */
