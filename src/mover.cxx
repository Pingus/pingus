//  $Id: mover.cxx,v 1.3 2003/10/18 23:17:27 grumbel Exp $
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

#include "mover.hxx"

namespace Pingus {

Mover::Mover(World* const world_arg, const Vector& pos_arg)
  : world(world_arg),
    pos(pos_arg),
    remaining_move(0.0f, 0.0f, 0.0f),
    collision(false)
{
}

Mover::~Mover ()
{
}

Vector Mover::get_pos() const
{
  return pos;
}

Vector Mover::remaining() const
{
  return remaining_move;
}

bool Mover::collided() const
{
  return collision;
}

} // namespace Pingus

/* EOF */
