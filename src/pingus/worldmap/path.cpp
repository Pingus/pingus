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

#include "pingus/worldmap/path.hpp"

namespace WorldmapNS {

Path::Path() :
  vec(),
  path_length_valid (false),
  path_length()
{
}

std::vector<Vector3f >::size_type
Path::size()
{
  return vec.size();
}

float
Path::length()
{
  if (path_length_valid)
  {
    return path_length;
  }
  else
  {
    path_length = calc_length ();
    path_length_valid = true;
    return path_length;
  }
}

float
Path::calc_length()
{
  if (vec.empty())
  {
    return 0.0f;
  }
  else
  {
    float length_ = 0;
    Vec::iterator prev = vec.begin();
    for(Vec::iterator next = prev + 1; next != vec.end(); ++next)
    {
      length_ += Vector3f ::distance2d(*prev, *next);
      prev = next;
    }
    return length_;
  }
}

Vector3f
Path::at(float vec_position)
{
  assert(!vec.empty());

  Path::iterator current = vec.begin ();
  Path::iterator next    = vec.begin () + 1;

  float comp_length = 0.0f;
  while (next != vec.end())
  {
    float length_ = Vector3f::distance2d(*current, *next);

    // The pingu is between current and next
    if (comp_length + length_ > vec_position)
    {
      float perc = (vec_position - comp_length) // length to walk from current node
        / length_;

      return Vector3f::interpolate(*current, *next, perc);
    }

    comp_length += length_;

    ++current;
    ++next;
  }
  return vec.back();
}

void
Path::reverse()
{
  std::reverse(vec.begin (), vec.end ());
}

} // namespace WorldmapNS

/* EOF */
