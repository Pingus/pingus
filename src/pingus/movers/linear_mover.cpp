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

#include "pingus/movers/linear_mover.hpp"

#include "pingus/collider.hpp"

namespace pingus::movers {

LinearMover::LinearMover(World const* world_arg, Vector2f const& pos_arg)
  : Mover(world_arg, pos_arg)
{
}

LinearMover::~LinearMover()
{
}

void
LinearMover::update(glm::vec2 const& move, Collider const& collision_check)
{
  int move_length = static_cast<int>(glm::length(move));

  // Make the step vector (i.e. change to a unit vector)
  glm::vec2 step_vector = glm::normalize(move);

  collision = false;

  // Move to the destination one unit vector at a time
  for (int i = 0; i < move_length && !collision; ++i)
  {
    collision = collision_check(world, pos, step_vector);

    pos = Vector2f(pos.as_vec() + step_vector);
  }

  // If on a collision pixel, back away from it.
  if (collision) {
    pos = Vector2f(pos.as_vec() - step_vector);
  }
}

} // namespace pingus::movers

/* EOF */
