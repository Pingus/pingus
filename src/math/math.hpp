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

#include <cassert>
#include <cmath>
#include <cstdlib>
#include <string>
#include <numbers>

namespace pingus {

/** A collection of small math helper functions, some of them might be
    equal in functionality to standard STL functions, but provided
    here for portability and broken STL implementations

    @brief A collection of mathematical helper functions */
namespace Math {

float const pi = std::numbers::pi_v<float>;
float const pi_2 = std::numbers::pi_v<float> / 2.0f;

inline
float frand()
{
  return static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) + 1.0f);
}

/** Write out the raw bits of a float as hex */
std::string float2string(float value);

/** Restore the raw bits of a float from a string */
float string2float(std::string const& str);

} // namespace Math

} // namespace pingus

#endif

/* EOF */
