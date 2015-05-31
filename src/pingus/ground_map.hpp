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

#ifndef HEADER_PINGUS_PINGUS_GROUND_MAP_HPP
#define HEADER_PINGUS_PINGUS_GROUND_MAP_HPP

#include <memory>

#include "engine/display/surface.hpp"
#include "pingus/globals.hpp"
#include "pingus/worldobj.hpp"

class SceneContext;
class CollisionMap;
class GroundMap;
class MapTile;

/** This map type is the defaulh maptype, it is should be used for the
    most levels. It allows to construct a map, from a set of simple
    small images, this allows the generation of large map without
    using to much diskspace. */
class GroundMap : public WorldObj
{
private:
  std::unique_ptr<CollisionMap> colmap;

  /** The tiles out of which the map is constructed */
  std::vector<std::shared_ptr<MapTile> > tiles;

  /** Width of the map */
  int width;
  /** Height of the map */
  int height;

  int tile_width;
  int tile_height;

public:
  GroundMap(int width, int height);
  virtual ~GroundMap();

  void draw(SceneContext& gc);

  CollisionMap* get_colmap();

  int  get_height();
  int  get_width();

  /** Put the gives surface provider onto the given coordinates */
  void put(Surface, int x, int y);

  /** Remove the gives surface provider onto the given coordinates
      (everything non-transparent that isn't Groundtype::GP_Solid
      is removed from the map) */
  void remove(Surface, int x, int y);

  float get_z_pos () const { return 0; }
  void set_pos(const Vector3f& p) {}
  Vector3f get_pos() const { return Vector3f(); }

  /** Low level version of the remove() call, acts on a single tile
      instead of the complete map-tiles */
  void put_alpha_surface(Surface provider, Surface sprovider,
                         int x, int y, int real_x, int real_y);

  MapTile* get_tile(int x, int y);
private:
  /** Draw the collision map onto the screen */
  void draw_colmap(SceneContext& gc);

  GroundMap (const GroundMap&);
  GroundMap& operator= (const GroundMap&);
};

#endif

/* EOF */
