//  $Id: WorldObj.hh,v 1.22 2002/01/15 10:48:49 grumbel Exp $
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

#ifndef WORLDOBJ_HH
#define WORLDOBJ_HH

#include "boost/smart_ptr.hpp"

class World;
class WorldObjData;

/** All objects that act in the world should be derived from this
 *  class, it provides access to all the important data from the world.
 *  Each world object has a $z$-position which indicates the depth of the
 *  object. By default, all world objects live on the same depth.
 */
class WorldObj
{
protected:
  /** The World all WorldObjects live in. */
  static World*  world;

public:
  /** Set the world pointer for all world objects */
  static void   set_world(World*);
  
  /** Return the current active world */
  static World* get_world () { return world; }

public:
  /** Creates a new WorldObj */
  WorldObj();

  /** Destroys a world object */
  virtual ~WorldObj();

  /** Returns the $z$-position of this object. */
  virtual float get_z_pos() const =0;

  /** Draws this WorldObject on the screen with the specified offset. If not
   *  overloaded, this method does nothing.
   *  (XXX: What are the bounds of this offset?)
   *  @param x the $x$-coordinate of this object
   *  @param y the $y$-coordinate of this object
   *  @param s the scalar by with the object is zoomed (1.0 is default)
   */
  virtual void draw_offset(int x, int y, float s = 1.0);

  /** Draws the objects collision map to the main collision map, this
   *  can be used for traps which need a solid ground. */
  virtual void draw_colmap();

  /** The update function is called once a game loop, the delta
   * specifies how much time is passed since the last update
   * delta = 1.0 means that one second of realtime has passed. */
  virtual void update(float delta);
};

#endif

/* EOF */
