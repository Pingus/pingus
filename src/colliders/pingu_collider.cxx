//  $Id: pingu_collider.cxx,v 1.1 2003/02/12 22:43:38 torangan Exp $
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

#include "../pingu_action.hxx"
#include "../groundtype.hxx"
#include "../vector.hxx"
#include "pingu_collider.hxx"

namespace Colliders {

PinguCollider::PinguCollider(const bool falling_arg, const int height_arg)
  : falling(falling_arg),
    height(height_arg)
{
}

PinguCollider::~PinguCollider()
{
}

bool PinguCollider::operator() (World* const world, Vector pos) const
{
  int pixel;
  bool collided = false;
  float top_of_pingu = pos.y - height;

  for (; pos.y >= top_of_pingu; --pos.y)
    {
      pixel = getpixel(world, pos);

      // If there is something in the way, then Pingu has collided with
      // something.  However, if not falling and colliding with a
      // Bridge, allow Pingu to go through it.
      if ((!falling || pixel != Groundtype::GP_BRIDGE)
	  && pixel != Groundtype::GP_NOTHING)
	{
	  collided = true;
	  break;
	}
    }

  return collided;
}

} // namespace Colliders

/* EOF */
