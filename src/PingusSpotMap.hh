//  $Id: PingusSpotMap.hh,v 1.4 2000/03/16 21:36:33 grumbel Exp $
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

#ifndef PingusSpotMap_HH
#define PingusSpotMap_HH

#include <fstream>
#include <string>
#include <vector>
#include <ClanLib/core.h>

#include "PinguMap.hh"
#include "ColMap.hh"
#include "PSMParser.hh"
#include "PLF.hh"

class MapTileSurface
{
private:
  bool empty;
  bool needs_reload;
public:
  CL_Surface* surface;

  MapTileSurface();
  virtual ~MapTileSurface();

  void reload(void);
  bool is_empty(void);
  void mark_dirty(void);
  void check_empty(void);
};

// This map type is the defaulh maptype, it is should be used for the
// most levels. It allows to construct a map, from a set of simple
// small images, this allows the generation of large map without using 
// to much diskspace.
class PingusSpotMap : public PinguMap
{
private:
  ColMap* colmap;
  typedef vector<MapTileSurface>::size_type TileIter;
  vector<surface_data> surfaces;
  PSMParser psm_parser;
  CL_Canvas* provider;
  CL_Surface*  map_surface;
  CL_SurfaceProvider* map_provider;
  vector<vector<MapTileSurface> > tile;
  int width;
  int height;

public:
  PingusSpotMap();
  PingusSpotMap(PLF*);
  virtual ~PingusSpotMap();

  ColMap* PingusSpotMap::get_colmap(void);
  void load(string);
  void load(PLF* plf);
  void gen_tiles(void);
  void draw(int x, int y, int w, int h, int of_x, int of_y, float s);
  int  get_height(void);
  int  get_width(void);
  CL_Surface* get_surface(void);
  void remove(CL_SurfaceProvider*, int, int);
  void put(CL_SurfaceProvider*, int, int);
  void create_maptiles();
};

#endif

/* EOF */
