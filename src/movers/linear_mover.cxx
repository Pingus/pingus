//  $Id: linear_mover.cxx,v 1.1 2003/02/12 22:43:38 torangan Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
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

#include "linear_mover.hxx"
#include "../collider.hxx"

namespace Movers {

LinearMover::LinearMover(World* const world_arg, const Vector& pos_arg)
  : Mover(world_arg, pos_arg)
{
}

LinearMover::~LinearMover()
{
}

void LinearMover::update(const Vector& move, const Collider& collision_at)
{
  float move_length = move.length();
  Vector target_pos = pos + move;
  Vector step_vector = move;

  // Make the step vector (i.e. change to a unit vector)
  step_vector.normalize();

  collision = false;

  // Move to the destination one unit vector at a time
  for (float i = 0; i < move_length && !collision; ++i)
    {
      pos += step_vector;

      collision = collision_at(world, pos);
    }

  // If on a collision pixel, back away from it.
  if (collision)
    pos -= step_vector;

  remaining_move = target_pos - pos;
}

} // namespace Movers

/* EOF */
