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

#ifndef HEADER_PINGUS_PINGUS_MOVER_HPP
#define HEADER_PINGUS_PINGUS_MOVER_HPP

#include "math/vector2f.hpp"

namespace pingus {

class Collider;
class World;

class Mover
{
public:
  /** Constructor of abstract class */
  Mover(World const* world_arg, Vector2f const& pos_arg);

  /** Destructor of abstract class */
  virtual ~Mover() = 0;

  /** Updates the position of the object taking into account collisions */
  virtual void update(glm::vec2 const& move, Collider const& collider) = 0;

  /** Get the resulting position vector */
  Vector2f get_pos() const;

  /** Get whether object stopped moving because it collided with something */
  bool collided() const;

protected:
  /** World in which the object should move */
  World const* world;

  /** Position of the object to move */
  Vector2f pos;

  /** Flag to denote whether object has had a collision */
  bool collision;

private:
  Mover(Mover const&);
  Mover & operator=(Mover const&);
};

} // namespace pingus

#endif

/* EOF */
