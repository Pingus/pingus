//  $Id: ColMap.hh,v 1.5 2000/05/03 16:49:44 grumbel Exp $
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

#ifndef COLMAP_HH
#define COLMAP_HH

#include <vector>
#include <string>

#include "ResDescriptor.hh"
#include "PSMParser.hh"

class ColMap
{
private:
  unsigned char* colmap;
  int    width;
  int   height;
  bool    init;
  
public:
  enum PixelStatus { 
	 NOTHING     = 0,
	 WALL        = 1<<0,
	 OUTOFSCREEN = 1<<1,
	 BRIDGE      = 1<<2,
	 SOLID       = 1<<3,
	 WATER       = 1<<4,
	 LAVA        = 1<<5
  };

  ColMap();
  ColMap(unsigned char* b, int w, int h);
  ~ColMap();

  unsigned char* get_data();
  int get_height();
  int get_width();

  int  load(unsigned char*, int w, int h);
  int  load(ResDescriptor desc);
  int  getpixel(int x, int y);

  void put(int x, int y, PixelStatus p = WALL);
  void put(CL_Surface*, int x, int y, surface_data::Type = surface_data::GROUND);
  void put(CL_SurfaceProvider*, int x, int y, surface_data::Type = surface_data::GROUND);

  // void remove(int x, int y);
  void remove(CL_Surface*, int x, int y);
  void remove(CL_SurfaceProvider*, int x, int y);

  void draw(int, int, float);
};

#endif /* COLMAP_HH */

/* EOF */
