//  $Id$
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

#ifndef HEADER_PINGUS_MATH_HPP
#define HEADER_PINGUS_MATH_HPP

#include <assert.h>
#include <stdlib.h>
#include "pingus.hpp"

/** A collection of small math helper functions, some of them might be
    equal in functionality to standard STL functions, but provided
    here for portability and broken STL implementations

    @brief A collection of mathematical helper functions */
namespace Math {

const double pi   = 3.14159265358979323846;	/* pi */
const double pi_2 = 1.57079632679489661923;	/* pi/2 */

// Win32 defines these are defines already, so we have to undef them
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

template<class T>
T min (const T& a, const T& b)
{
  if (a < b)
    return a;
  else
    return b;
}

template<class T>
T max (const T& a, const T& b)
{
  if (a > b)
    return a;
  else
    return b;
}

template<class T>
T clamp (const T& low, const T& v, const T& high)
{
  assert(low <= high);
  return max((low), min((v), (high)));
}

inline
float frand()
{
  return rand() / (RAND_MAX + 1.0f);
}

inline
bool rand_bool()
{
  return rand()%2 == 0;
}

inline
int round (float f)
{
  if (f >= 0.0f)
    return int(f + 0.5f);
  else
    return int(f - 0.5f);
}

} // namespace Math

#endif

/* EOF */
