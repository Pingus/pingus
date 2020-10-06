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

#ifndef HEADER_PINGUS_MATH_MATH_HPP
#define HEADER_PINGUS_MATH_MATH_HPP

#include <assert.h>
#include <stdlib.h>
#include <string>

/** A collection of small math helper functions, some of them might be
    equal in functionality to standard STL functions, but provided
    here for portability and broken STL implementations

    @brief A collection of mathematical helper functions */
namespace Math {

const float pi   = 3.14159265358979323846f;     /* pi */
const float pi_2 = 1.57079632679489661923f;     /* pi/2 */

inline
float frand()
{
  return static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) + 1.0f);
}

inline
bool rand_bool()
{
  return rand()%2 == 0;
}

inline int round (float f)
{
  if (f >= 0.0f)
    return int(f + 0.5f); // NOLINT
  else
    return int(f - 0.5f);
}

float abs(float v);
float sin(float a);
float cos(float a);
float sqrt(float a);
float mod(float x, float y);
float floor(float x);
float atan2(float x, float y);

/** Write out the raw bits of a float as hex */
std::string float2string(float value);

/** Restore the raw bits of a float from a string */
float string2float(const std::string& str);

} // namespace Math

#endif

/* EOF */
