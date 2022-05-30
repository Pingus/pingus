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

#ifndef HEADER_PINGUS_PINGUS_MOVERS_LINEAR_MOVER_HPP
#define HEADER_PINGUS_PINGUS_MOVERS_LINEAR_MOVER_HPP

#include "pingus/mover.hpp"

class Collider;

namespace pingus::movers {

class LinearMover : public Mover
{
public:
  /** Constructor */
  LinearMover(World const* world_arg, Vector2f const& pos_arg);

  /** Destructor */
  ~LinearMover() override;

  /** Updates the position of the object taking into account collisions */
  void update(glm::vec2 const& move, Collider const& collision_at) override;
};

} // namespace pingus::movers

#endif

/* EOF */
