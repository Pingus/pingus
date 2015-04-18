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

#include "math/vector3f.hpp"

class Collider;
class World;

class Mover
{
public:
  /** Constructor of abstract class */
  Mover(World* const world_arg, const Vector3f& pos_arg);

  /** Destructor of abstract class */
  virtual ~Mover() = 0;

  /** Updates the position of the object taking into account collisions */
  virtual void update(const Vector3f& move, const Collider& collider) = 0;

  /** Get the resulting position vector */
  Vector3f get_pos() const;

  /** Get whether object stopped moving because it collided with something */
  bool collided() const;

protected:
  /** World in which the object should move */
  World* const world;

  /** Position of the object to move */
  Vector3f pos;

  /** Flag to denote whether object has had a collision */
  bool collision;

private:
  Mover(const Mover&);
  Mover & operator=(const Mover&);
};

#endif

/* EOF */
