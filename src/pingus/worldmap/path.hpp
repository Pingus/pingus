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

#ifndef HEADER_PINGUS_PINGUS_WORLDMAP_PATH_HPP
#define HEADER_PINGUS_PINGUS_WORLDMAP_PATH_HPP

#include <algorithm>
#include <vector>

#include "math/vector3f.hpp"

namespace WorldmapNS {

/** a wrapper class around std::vector<Vector>, which can calculate
    the length of the path and a few other things */
class Path
{
private:
  typedef std::vector<Vector3f> Vec;
  Vec vec;

  bool  path_length_valid;
  float path_length;

private:
  float calc_length();
public:
  typedef Vec::iterator iterator;
  typedef Vec::reverse_iterator reverse_iterator;

  Path();

  /** Return the number of nodes */
  Vec::size_type size();

  /** @return the length of the path in pixels */
  float length();

  /** walk a distance of vec_position on the path and return the position,
      positions between two Vectors are interpolated, if length is
      larger then path *back() will be returned */
  Vector3f at(float vec_position);

  bool empty() { return vec.empty(); }
  void clear() { path_length_valid = false; vec.clear(); }

  iterator begin() { return vec.begin(); }
  iterator end() { return vec.end(); }
  reverse_iterator rbegin() { return vec.rbegin(); }
  reverse_iterator rend() { return vec.rend(); }

  void push_back(const Vector3f& v) {
    path_length_valid = false;
    vec.push_back(v);
  }

  void insert(Path& p) {
    path_length_valid = false;
    //vec.insert(vec.end(), p.vec.begin(), p.vec.end());
    vec.reserve(vec.size() + p.vec.size ());
    std::copy(p.vec.begin(), p.vec.end(), std::back_inserter(vec));
  }

  void reverse_insert(Path& p) {
    path_length_valid = false;
    //vec.insert(vec.end(), p.vec.rbegin(), p.vec.rend());
    vec.reserve(vec.size() + p.vec.size ());
    std::copy(p.vec.rbegin(), p.vec.rend(), std::back_inserter(vec));
  }

  void reverse();
};

} // namespace WorldmapNS

#endif

/* EOF */
