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

#ifndef HEADER_PINGUS_PINGUS_WORLDOBJ_HPP
#define HEADER_PINGUS_PINGUS_WORLDOBJ_HPP

#include "engine/display/sprite.hpp"
#include "math/vector3f.hpp"
#include "util/reader.hpp"

class ReaderMapping;
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
  WorldObj(const ReaderMapping& reader);

  WorldObj (const WorldObj&) : id() {}
  WorldObj& operator= (const WorldObj&) { return *this; }

  /** Destroys a world object */
  virtual ~WorldObj ();

  std::string get_id() const { return id; }

  /** Returns the $z$-position of this object. */
  virtual float get_z_pos () const =0;

  virtual void set_pos(const Vector3f& p) = 0;
  virtual Vector3f get_pos() const = 0;

  /** Draw the WorldObj to the given SceneContext */
  virtual void draw(SceneContext& gc) = 0;
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

