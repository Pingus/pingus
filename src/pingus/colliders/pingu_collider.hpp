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

#ifndef HEADER_PINGUS_PINGUS_COLLIDERS_PINGU_COLLIDER_HPP
#define HEADER_PINGUS_PINGUS_COLLIDERS_PINGU_COLLIDER_HPP

#include "pingus/collider.hpp"

namespace Colliders {

class PinguCollider : public Collider
{
public:
  /** Constructor */
  PinguCollider(const int height_arg);

  /** Destructor */
  ~PinguCollider();

  /** Find out if a Pingu at the specified position is colliding with
      something */
  bool operator() (World* const world, Vector3f current_pos,
                   const Vector3f& step_vector) const;

private:
  /** Pingu could be on its belly.  Therefore, this is the current height of
      the Pingu. */
  int height;
};

} // namespace Colliders

#endif

/* EOF */
