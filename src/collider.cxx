//  $Id: collider.cxx,v 1.4 2003/10/18 23:17:27 grumbel Exp $
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

#include "collider.hxx"
#include "col_map.hxx"
#include "math/vector3f.hpp"
#include "world.hxx"

Collider::Collider()
{
}

Collider::~Collider()
{
}

bool Collider::operator() (World* const world, Vector3f current_pos,
			    const Vector3f& step_vector) const
{
  UNUSED_ARG(world);
  UNUSED_ARG(current_pos);
  UNUSED_ARG(step_vector);

  return false;
}

int Collider::getpixel(World* const world, const Vector3f& pos) const
{
  return world->get_colmap()->getpixel(static_cast<int>(pos.x),
                                       static_cast<int>(pos.y));
}

/* EOF */
