//  $Id: worldobj.hpp 3582 2008-06-29 16:56:56Z grumbel $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_PINGUS_WORLDOBJ_HPP
#define HEADER_PINGUS_WORLDOBJ_HPP

#include "pingus.hpp"
#include "file_reader.hpp"
#include "sprite.hpp"
#include "collision_mask.hpp"

class FileReader;
class SceneContext;
class SmallMap;
class World;

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

private:
  std::string id;

public:
  /** Creates a new WorldObj*/
  WorldObj ();
  WorldObj(const FileReader& reader);

  WorldObj (const WorldObj&)       { }
  WorldObj& operator= (const WorldObj&) { return *this; }

  /** Destroys a world object */
  virtual ~WorldObj ();

  std::string get_id() const { return id; }

  /** Returns the $z$-position of this object. */
  virtual float get_z_pos () const =0;

  /** Draws this WorldObject on the screen with the specified offset. If not
   *  overloaded, this method does nothing.
   *  (XXX: What are the bounds of this offset?)
   *  @param x the $x$-coordinate of this object
   *  @param y the $y$-coordinate of this object
   *  @param s the scalar by with the object is zoomed (1.0 is default)
   */
  virtual void draw_offset (int x, int y, float s = 1.0);

  /** Draw the WorldObj to the given SceneContext */
  virtual void draw(SceneContext& gc);
  virtual void draw_smallmap(SmallMap* smallmap);

  /** Draws the objects collision map to the main collision map, draws
      stuff onto the gfx map or do other manipulations to the World */
  virtual void on_startup ();

  /** @return true if this WorldObj is empty and doesn't have an
      update() or draw() function, but only a on_startup() one. The
      World can so decide which objects need to stay active and which
      one can get purged after calling on_startup() */
  virtual bool purge_after_startup() { return false; }

  /** The update function is called once a game loop, the delta
   * specifies how much time is passed since the last update
   * delta = 1.0 means that one second of realtime has passed. */
  virtual void update ();
};


#endif

/* EOF */

