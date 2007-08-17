//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_LINEAR_MOVER_HXX
#define HEADER_PINGUS_LINEAR_MOVER_HXX

#include "../mover.hpp"


class Collider;

namespace Movers {

class LinearMover : public Mover
{
  public:
    /** Constructor */
    LinearMover(World* const world_arg, const Vector3f& pos_arg);

    /** Destructor */
    ~LinearMover();

    /** Updates the position of the object taking into account collisions */
    void update(const Vector3f& move, const Collider& collision_at);
};

} // namespace Movers

#endif

/* EOF */
