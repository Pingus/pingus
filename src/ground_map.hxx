//  $Id: spot_map.hxx,v 1.11 2003/10/22 11:11:22 grumbel Exp $
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
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
//  02111-1307, USA.

#ifndef HEADER_PINGUS_GROUND_MAP_HXX
#define HEADER_PINGUS_GROUND_MAP_HXX

#include <vector>
#include <ClanLib/Display/pixel_buffer.h>
#include <ClanLib/Display/surface.h>
#include "globals.hxx"
#include "worldobj.hxx"
#include "ground_map.hxx"

class CL_PixelBuffer;

namespace Pingus {

class SceneContext;
class PingusLevel;
class ColMap;
class GroundMap;

class MapTile
{
private:
  CL_Surface     surface;
  CL_PixelBuffer pixelbuffer;

  void prepare();
public:
  MapTile();
  ~MapTile();

	void remove(CL_PixelBuffer, int x, int y, int real_x, int real_y, GroundMap*);  
  void put(CL_PixelBuffer, int x, int y);  

  CL_Surface get_surface() const { return surface; }
};

/** This map type is the defaulh maptype, it is should be used for the
    most levels. It allows to construct a map, from a set of simple
    small images, this allows the generation of large map without
    using to much diskspace. */
class GroundMap : public WorldObj
{
private:
  ColMap* colmap;

  /** The tiles out of which the map is constructed */
  std::vector<std::vector<MapTile> > tile;

  /** Width of the map */
  int width;
  /** Height of the map */
  int height;

  int tile_width;
  int tile_height;
public:
  GroundMap(const PingusLevel& arg_plf);
  virtual ~GroundMap();

  void draw(SceneContext& gc);

  ColMap* get_colmap();

  int  get_height();
  int  get_width();

  /** Put the gives surface provider onto the given coordinates */
  void put(CL_PixelBuffer, int x, int y);

  /** Remove the gives surface provider onto the given coordinates
      (everything non-transparent that isn't Groundtype::GP_Solid 
			is removed from the map) */
  void remove(CL_PixelBuffer, int x, int y);

  float get_z_pos () const { return 0; }

	/** Low level version of the remove() call, acts on a single tile
      instead of the complete map-tiles */
  void put_alpha_surface(CL_PixelBuffer provider, CL_PixelBuffer sprovider,
			 int x, int y, int real_x, int real_y);

private:
  /** Draw the collision map onto the screen */
  void draw_colmap(SceneContext& gc);

  GroundMap (const GroundMap&);
  GroundMap& operator= (const GroundMap&);
};

} // namespace Pingus

#endif

/* EOF */
