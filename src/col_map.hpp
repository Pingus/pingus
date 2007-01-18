//  $Id: col_map.hxx,v 1.15 2003/10/21 21:37:06 grumbel Exp $
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

#ifndef HEADER_PINGUS_COL_MAP_HXX
#define HEADER_PINGUS_COL_MAP_HXX

#include "groundtype.hpp"

class CL_Surface;
class PixelBuffer;

class DrawingContext;
class ResDescriptor;
class PixelStatus;

// Collsion Map
/** The collision map is used to represent the enviroment where the
    Pingus walk around. The ground can have different properties, it
    can contain lava or water, it can be solid and many more. */
class ColMap
{
private:
  /** The serial number indicates the state of the colmap, on every
      change of the colmap it will get increased. */
  unsigned int serial;

  /** The width of the collision map. */
  int    width;

  /** The height of the collision map. */
  int   height;

  /** A array of uchar, each uchar represents a pixel on the map. */
  unsigned char* colmap;

public:
  /** Init the colmap from a given area of memory.
      The memory will be deleted in the destructor. */
  ColMap(int w, int h);

  /** delete[] the uchar array used for the colmap */
  ~ColMap();

  /** Returns the raw uchar array used for the inner representation of
      the colmap. This is used by the smallmap to create the radar  */
  unsigned char* get_data();

  /** Returns the height of the collision map. */
  int get_height();

  /** Returns the height of the collision map. */
  int get_width();

  int  getpixel(int x, int y);

  /** @return a number which represents the state of the collision
      map, once it changes the serial changes also */
  unsigned get_serial();

  /** Return true if the given GroundType i*/
  bool blit_allowed (int x, int y,  Groundtype::GPType);

  void put(int x, int y, Groundtype::GPType p = Groundtype::GP_GROUND);
  void put(PixelBuffer, int x, int y, Groundtype::GPType);

  void remove(int x, int y);
  void remove(PixelBuffer, int x, int y);

  void draw(DrawingContext& gc);

private:
  ColMap (const ColMap&);
  ColMap& operator= (const ColMap&);
};


#endif /* COLMAP_HH */

/* EOF */
