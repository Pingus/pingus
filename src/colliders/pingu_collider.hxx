//  $Id: pingu_collider.hxx,v 1.1 2003/02/12 22:43:38 torangan Exp $
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

#ifndef HEADER_PINGUS_UPRIGHT_PINGU_COLLIDER_HXX
#define HEADER_PINGUS_UPRIGHT_PINGU_COLLIDER_HXX

#include "../collider.hxx"

namespace Colliders {

class PinguCollider : public Collider
{
  public:
    /** Constructor for abstract class */
    PinguCollider(const bool falling_arg, const int height_arg);

    /** Destructor for abstract class */
    ~PinguCollider();

    /** Find out if a Pingu at the specified position is colliding with
        something */
    bool operator() (World* const world, Vector pos) const;

  private:
    /** Indicates whether a Pingu is falling or not */
    bool falling;

    /** Pingu could be on its belly.  Therefore, this is the current height of
	the Pingu. */
    int height;
};

} // namespace Colliders

#endif

/* EOF */
