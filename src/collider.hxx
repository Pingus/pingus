//  $Id: collider.hxx,v 1.4 2003/10/18 23:17:27 grumbel Exp $
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

#ifndef HEADER_PINGUS_COLLIDER_HXX
#define HEADER_PINGUS_COLLIDER_HXX


class Vector;
class World;

class Collider
{
  public:
    /** Constructor for abstract class */
    Collider();

    /** Destructor for abstract class */
    virtual ~Collider() = 0;

    /** Find out if object will collide with something */
    virtual bool operator() (World* const world, Vector current_pos,
			      const Vector& step_vector) const = 0;

  protected:
    /** Get the Collision Map pixel at the specified position in the specified
	world */
    int getpixel(World* const world, const Vector& pos) const;
};


#endif

/* EOF */
