//  $Id: spot_map.cxx,v 1.31 2003/10/22 11:11:22 grumbel Exp $
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

#include <stdio.h>
#include <iostream>
#include <ClanLib/Display/pixel_buffer.h>
#include <ClanLib/Display/pixel_format.h>
#include "gui/graphic_context.hxx"
#include "plf.hxx"
#include "pingus_error.hxx"
#include "blitter.hxx"
#include "spot_map.hxx"
#include "gettext.h"
#include "col_map.hxx"
#include "math.hxx"

namespace Pingus {

using namespace WorldObjsData;

MapTile::MapTile () 
{
}

MapTile::~MapTile()
{
}

void
MapTile::prepare()
{
  if (!pixelbuffer)
    {
      pixelbuffer = CL_PixelBuffer(tile_size, tile_size, tile_size*4, CL_PixelFormat::rgba8888);
      Blitter::clear_canvas(pixelbuffer);
    }
}

void
MapTile::remove(CL_PixelBuffer obj, int x, int y)
{
  if (surface)
    {
      Blitter::put_alpha_surface(pixelbuffer, obj, x, y);
      surface = CL_Surface(new CL_PixelBuffer(pixelbuffer), true);
    }
}

void
MapTile::put(CL_PixelBuffer obj, int x, int y)
{
  prepare();
  Blitter::put_surface(pixelbuffer, obj, x, y);
  surface = CL_Surface(new CL_PixelBuffer(pixelbuffer), true);
}

PingusSpotMap::PingusSpotMap(PLF* plf)
{
  width  = plf->get_width();
  height = plf->get_height();

  colmap = new ColMap(width, height);

  // Checking that the map has the correct size, only multiples of
  // tile_size are allowed, anything else wouldn't fit very well on
  // the colmap
  if ((width % tile_size) != 0)
    {
      width += (tile_size - (width % tile_size));
    }

  if ((height % tile_size) != 0)
    {
      height += (tile_size - (height % tile_size));
    }

  tile_width  = width/tile_size;
  tile_height = height/tile_size;

  // Allocating tile map
  tile.resize(tile_width);
  for(unsigned int i=0; i < tile.size(); ++i)
    tile[i].resize(tile_height);

  // fix the height back to the correct values
  width  = plf->get_width();
  height = plf->get_height();
}

PingusSpotMap::~PingusSpotMap(void)
{
  delete colmap;
}

void
PingusSpotMap::draw_colmap(GraphicContext& gc)
{
  colmap->draw(gc);
}

// Draws the map with a offset, needed for scrolling
void
PingusSpotMap::draw(GraphicContext& gc)
{
  const CL_Rect& display = gc.get_clip_rect();

  // FIXME: delete the next four lines and replace them with gc.get_clip_rect()
  if (draw_collision_map)
    {
      draw_colmap(gc);
    }
  else
    {
      // Trying to calc which parts of the tilemap needs to be drawn
      int start_x = Math::max(0, display.left/tile_size);
      int start_y = Math::max(0, display.top/tile_size);
      int tilemap_width  = display.get_width()  / tile_size + 1;
      int tilemap_height = display.get_height() / tile_size + 1;

      // drawing the stuff
      for (int x = start_x; x <= (start_x + tilemap_width) && x < int(tile.size()); ++x)
        for (int y = start_y; y <= start_y + tilemap_height && y < int(tile[x].size()); ++y)
          {
            if (tile[x][y].get_surface())
              {
                gc.draw(tile[x][y].get_surface(),
                        Vector(x * tile_size, y * tile_size));
              }
            else
              {
                if (pingus_debug_flags & PINGUS_DEBUG_TILES)
                  gc.draw_fillrect(x * tile_size, y * tile_size,
                                   x * tile_size + tile_size, y * tile_size + tile_size,
                                   1.0f, 0.0f, 0.0f, 0.3f);
              }
          }
    }
}

// Returns the width of the map, it is read directly from the *.psm file
int
PingusSpotMap::get_width(void)
{
  return width;
}

// Returns the height of the map, it is read directly from the *.psm file
int
PingusSpotMap::get_height(void)
{
  return height;
}

void
PingusSpotMap::remove(CL_PixelBuffer sprovider, int x, int y)
{
  // Get the start tile and end tile
  int start_x = Math::max(x / tile_size, 0);
  int start_y = Math::max(y / tile_size, 0);
  int end_x   = Math::min((x + sprovider.get_width()) / tile_size,
                          (width - 1) / tile_size);
  int end_y   = Math::min((y + sprovider.get_height()) / tile_size,
                          (height - 1) / tile_size);

  for(int ix = start_x; ix <= end_x; ++ix)
    for(int iy = start_y; iy <= end_y; ++iy)
      {
        tile[ix][iy].remove(sprovider, x - (ix * tile_size),
                            y - (iy * tile_size));
      }
}

void
PingusSpotMap::put_alpha_surface(CL_PixelBuffer provider, CL_PixelBuffer sprovider,
				 int x, int y, int real_x_arg, int real_y_arg)
{
  int start_i;
  unsigned char* tbuffer; // Target buffer
  int twidth, theight, tpitch;

  unsigned char* sbuffer; // Source buffer
  int swidth, sheight, spitch;

  int x_offset, y_offset;

  int real_x;
  int real_y;

  //  assert(sprovider->get_depth() == 8);
  if (sprovider.get_format().get_depth() != 8)
    {
      char str[128];
      snprintf(str, 128, _("Image has wrong color depth: %d"), sprovider.get_format().get_depth());
      PingusError::raise(str);
    }
  //  assert(provider->get_pixel_format() == RGBA8888);

  provider.lock();
  sprovider.lock();

  tbuffer = static_cast<unsigned char*>(provider.get_data());
  sbuffer = static_cast<unsigned char*>(sprovider.get_data());

  twidth = provider.get_width();
  theight = provider.get_height();
  tpitch = provider.get_pitch();

  swidth = sprovider.get_width();
  sheight = sprovider.get_height();
  spitch = sprovider.get_pitch();

  if (y < 0) {
    y_offset = 0-y;
  } else {
    y_offset = 0;
  }

  if (x < 0) {
    x_offset = -x;
  } else {
    x_offset = 0;
  }

  real_y = real_y_arg;
  real_x = real_x_arg;

  for(int line=y_offset; line < sheight && (line + y) < theight; ++line)
    {
      start_i = ((line + y) * tpitch) + (x*4);

      real_x = real_x_arg;
      for(int i=start_i+(4*x_offset),j=line*spitch+x_offset;
	  i < start_i + (4*swidth) && (i-start_i+(x*4)) < (4*twidth); i+=4,++j)
	{
	  if (sbuffer[j])
	    {
	      if (pingus_debug_flags & PINGUS_DEBUG_ACTIONS)
		{
		  if (!(colmap->getpixel(real_x, real_y) == Groundtype::GP_SOLID))
		    {
		      tbuffer[i + 0] = 255;
		      tbuffer[i + 1] = 255;
		      tbuffer[i + 2] = 255;
		      tbuffer[i + 3] = 255;
		    }
		  else
		    {
		      tbuffer[i + 0] = 255;
		      tbuffer[i + 1] = 255;
		      tbuffer[i + 2] = 0;
		      tbuffer[i + 3] = 0;
		    }
		}
	      else
		{
		  if (!(colmap->getpixel(real_x, real_y) == Groundtype::GP_SOLID))
		    {
		      tbuffer[i + 0] = 0;
		    }
		}
	    }
	  ++real_x;
	}
      ++real_y;
    }

  sprovider.unlock();
  provider.unlock();
}

void
PingusSpotMap::put(CL_PixelBuffer sprovider, int x, int y)
{
  // Get the start tile and end tile
  int start_x = std::max(0, x / tile_size);
  int start_y = std::max(0, y / tile_size);
  int end_x   = std::min(tile_width,  (x + sprovider.get_width())  / tile_size + 1);
  int end_y   = std::min(tile_height, (y + sprovider.get_height()) / tile_size + 1);

  for(int ix = start_x; ix < end_x; ++ix)
    for(int iy = start_y; iy < end_y; ++iy)
      {
        tile[ix][iy].put(sprovider,
                         x - (ix * tile_size), y - (iy * tile_size));
      }
}

ColMap*
PingusSpotMap::get_colmap(void)
{
  return colmap;
}

} // namespace Pingus

/* EOF */
