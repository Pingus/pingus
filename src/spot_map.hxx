//  $Id: spot_map.hxx,v 1.5 2002/09/04 19:40:19 grumbel Exp $
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

#ifndef HEADER_PINGUS_SPOT_MAP_HXX
#define HEADER_PINGUS_SPOT_MAP_HXX

#include <vector>
#include "globals.hxx"
#include "pingu_map.hxx"
#include "groundpiece_data.hxx"

class PLF;
class ColMap;
class CL_Canvas;

class MapTileSurface
{
private:
  bool empty;
public:
  CL_Surface surface;

  MapTileSurface ();
  virtual ~MapTileSurface ();
  
  MapTileSurface (const MapTileSurface& old);
  MapTileSurface operator= (const MapTileSurface& old);

  void reload (void);
  inline bool is_empty (void) { return empty; }
  void mark_dirty (void);
  void check_empty (void);
  void set_empty (bool);
};

/** This map type is the defaulh maptype, it is should be used for the
    most levels. It allows to construct a map, from a set of simple
    small images, this allows the generation of large map without
    using to much diskspace. */
class PingusSpotMap : public PinguMap
{
private:
  ColMap* colmap;

  typedef std::vector<MapTileSurface>::size_type TileIter;

  std::vector<GroundpieceData> surfaces;

  CL_Canvas* provider;
  CL_Surface map_surface;
  CL_Canvas* map_canvas;

  std::vector<std::vector<MapTileSurface> > tile;

  int width;
  int height;

public:
  PingusSpotMap(PLF*);
  virtual ~PingusSpotMap();

  void draw(GraphicContext& gc);

  void generate_colmap();
  ColMap* get_colmap();
  void load(PLF* plf);
  void gen_tiles();

  int  get_height(void);
  int  get_width(void);
  CL_Surface get_surface(void);
  void remove(CL_SurfaceProvider*, int, int);
  void put_alpha_surface(CL_Canvas* provider, CL_SurfaceProvider* sprovider,
			 int x, int y, int real_x, int real_y);
  void put(CL_SurfaceProvider*, int, int);
  void create_maptiles();
  void create_map();
  void mark_tiles_not_empty(int, int, int, int);

  float get_z_pos () const { return 0; }
  
private:
  /** Draw the collision map onto the screen */
  void draw_colmap(GraphicContext& gc);

  PingusSpotMap (const PingusSpotMap&);
  PingusSpotMap operator= (const PingusSpotMap&);
};

#endif

/* EOF */
