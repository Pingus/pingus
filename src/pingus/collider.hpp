// Pingus - A free Lemmings clone
// Copyright (C) 1999 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_PINGUS_COLLIDER_HPP
#define HEADER_PINGUS_PINGUS_COLLIDER_HPP

#include <glm/glm.hpp>
#include "math/vector2f.hpp"

namespace pingus {

class World;

class Collider
{
public:
  /** Constructor for abstract class */
  Collider();

  /** Destructor for abstract class */
  virtual ~Collider() = 0;

  /** Find out if object will collide with something */
  virtual bool operator() (World const* world, Vector2f const& current_pos,
                           glm::vec2 const& step_vector) const = 0;

protected:
  /** Get the Collision Map pixel at the specified position in the specified
      world */
  int getpixel(World const* world, Vector2f const& pos) const;
};

} // namespace pingus

#endif

/* EOF */
