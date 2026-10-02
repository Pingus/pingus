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

#include <prio/fwd.hpp>

#include "engine/display/sprite.hpp"
#include "math/vector2f.hpp"
#include "util/reader.hpp"

namespace pingus {

class SceneContext;
class SmallMap;
class World;

/** Base class of the parts of the World that are not level object
 *  entities: the ground, the pingus and the particle systems. Level
 *  objects (traps, exits, backgrounds, ...) are entities, see
 *  pingus/ecs/. Each world object has a z-position which indicates the
 *  depth of the object.
 */
class WorldObj
{
protected:
  /** The World the object is part of, nullptr for objects that don't
      need it */
  World* world;

public:
  explicit WorldObj(World* world_ = nullptr);
  virtual ~WorldObj();

  WorldObj(WorldObj const&) = delete;
  WorldObj& operator=(WorldObj const&) = delete;

  /** Returns the $z$-position of this object. */
  virtual float z_index() const =0;
  virtual void set_z_index(float z_index) =0;

  virtual void set_pos(Vector2f const& p) = 0;
  virtual Vector2f get_pos() const = 0;

  /** Draw the WorldObj to the given SceneContext */
  virtual void draw(SceneContext& gc) = 0;
  virtual void draw_smallmap(SmallMap* smallmap);

  /** Draws the objects collision map to the main collision map, draws
      stuff onto the gfx map or do other manipulations to the World */
  virtual void on_startup();

  /** The update function is called once a game loop, the delta
   * specifies how much time is passed since the last update
   * delta = 1.0 means that one second of realtime has passed. */
  virtual void update();
};

} // namespace pingus

#endif

/* EOF */

