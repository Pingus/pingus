//  $Id: spot_map.cxx,v 1.26 2003/04/19 10:23:17 torangan Exp $
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
#include <ClanLib/Display/SurfaceProviders/canvas.h>
#include "gui/graphic_context.hxx"
#include "plf.hxx"
#include "pingus_error.hxx"
#include "blitter.hxx"
#include "spot_map.hxx"
#include "my_gettext.hxx"
#include "col_map.hxx"
#include "math.hxx"

using namespace WorldObjsData;

MapTileSurface::MapTileSurface () : empty(true)
{
}

MapTileSurface::~MapTileSurface()
{
}

MapTileSurface::MapTileSurface (const MapTileSurface& old) : empty(old.empty),
                                                             surface(old.surface)
{
}

MapTileSurface&
MapTileSurface::operator= (const MapTileSurface& old)
{
  if (this == &old)
    return *this;

  empty   = old.empty;
  surface = old.surface;

  return *this;
}

void
MapTileSurface::set_empty(bool e)
{
  empty = e;
}

void
MapTileSurface::reload(void)
{
  surface.reload();
}

void
MapTileSurface::mark_dirty()
{
  empty = false;
}

void
MapTileSurface::check_empty()
{
  // FIXME: obsolete
  CL_SurfaceProvider* provider = surface.get_provider();
  unsigned char* buffer;
  int lenght;

  empty = true;

  provider->lock();
  lenght = provider->get_pitch() * provider->get_height();
  buffer = static_cast<unsigned char*>(provider->get_data());

  // Jumping 4 steps because of RGBA
  for(int i=0; i < lenght; i += 4) {
    if (buffer[i] != 0) {
      empty = false;
      return;
    }
  }

  surface.get_provider()->unlock();
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

  // Allocating tile map
  tile.resize(width/tile_size);
  for(TileIter i=0; i < tile.size(); ++i)
    tile[i].resize(height/tile_size);

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
#if 0
  { // calculate number of used/empty tiles
    int tiles_total = 0;
    int tiles_empty = 0;
    int tiles_used  = 0;
    for(TileIter x=0; x < tile.size(); ++x)
      {
	for(TileIter y=0; y < tile[x].size(); ++y)
	  {
	    if (tile[x][y].is_empty())
	      ++tiles_empty;
	    else
	      ++tiles_used;
	    ++tiles_total;
	  }
      }
    std::cout << "Tiles: " << tiles_total << " " << tiles_empty << "/" << tiles_used << std::endl;
    std::cout << "   " << float(tiles_used)/float(tiles_total) * 100.0f << "% of the map are used" << std::endl;
  }
#endif
  const CL_Rect& display = gc.get_clip_rect();

  // FIXME: delete the next four lines and replace them with gc.get_clip_rect()
  if (draw_collision_map)
    {
      draw_colmap(gc);
    }
  else
    {
      // Trying to calc which parts of the tilemap needs to be drawn
      int start_x = Math::max(0, display.x1/tile_size);
      int start_y = Math::max(0, display.y1/tile_size);
      unsigned int tilemap_width  = display.get_width()  / tile_size + 1;
      unsigned int tilemap_height = display.get_height() / tile_size + 1;

      // drawing the stuff
      for (TileIter x = start_x;
	   x <= (start_x + tilemap_width) && x < tile.size();
	   ++x)
	{
	  for (TileIter y = start_y;
	       y <= start_y + tilemap_height && y < tile[x].size();
	       ++y)
	    {
	      if (!tile[x][y].is_empty())
		{
		  gc.draw(tile[x][y].surface,
			  x * tile_size,
			  y * tile_size);
#ifdef PINGUS_EARTHQUAKE
		  gc.draw(tile[x][y].surface,
			  x * tile_size + rand ()%20-10,
			  y * tile_size + rand ()%20-10);
#endif
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
PingusSpotMap::remove(CL_SurfaceProvider* sprovider, int x, int y)
{
  // Get the start tile and end tile
  int start_x = Math::max(x / tile_size, 0);
  int start_y = Math::max(y / tile_size, 0);
  int end_x   = Math::min((x + sprovider->get_width()) / tile_size,
                          static_cast<unsigned int>((width - 1) / tile_size));
  int end_y   = Math::min((y + sprovider->get_height()) / tile_size,
                          static_cast<unsigned int>((height - 1) / tile_size));



  for(int ix = start_x; ix <= end_x; ++ix)
    {
      for(int iy = start_y; iy <= end_y; ++iy)
	{
	  if (!tile[ix][iy].is_empty())
	    {
	      put_alpha_surface(static_cast<CL_Canvas*>(tile[ix][iy].surface.get_provider()),
				sprovider,
				x - (ix * tile_size), y - (iy * tile_size),
				// FIXME, I am broken
				Math::max(x, ix * tile_size),
				Math::max(y, iy * tile_size));
	      tile[ix][iy].reload();
	    }
	}
    }
}

void
PingusSpotMap::put_alpha_surface(CL_Canvas* provider, CL_SurfaceProvider* sprovider,
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
  if (sprovider->get_depth() != 8)
    {
      char str[128];
      snprintf(str, 128, _("Image has wrong color depth: %d"), sprovider->get_depth());
      PingusError::raise(str);
    }
  //  assert(provider->get_pixel_format() == RGBA8888);

  provider->lock();
  sprovider->lock();

  tbuffer = static_cast<unsigned char*>(provider->get_data());
  sbuffer = static_cast<unsigned char*>(sprovider->get_data());

  twidth = provider->get_width();
  theight = provider->get_height();
  tpitch = provider->get_pitch();

  swidth = sprovider->get_width();
  sheight = sprovider->get_height();
  spitch = sprovider->get_pitch();

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

  sprovider->unlock();
  provider->unlock();
}

void
PingusSpotMap::put(CL_SurfaceProvider* sprovider, int x, int y)
{
  // Get the start tile and end tile
  int start_x = x / tile_size;
  int start_y = y / tile_size;
  int end_x = (x + sprovider->get_width()) / tile_size;
  int end_y = (y + sprovider->get_height()) / tile_size;

  if (start_x < 0)
    start_x = 0;
  if (start_y < 0)
    start_y = 0;

  if (end_x > (int) tile.size() - 1)
    end_x = tile.size() - 1;
  if (end_y > (int) tile[0].size() - 1)
    end_y = tile[0].size() - 1;

  for(int ix = start_x; ix <= end_x; ++ix)
    {
      for(int iy = start_y; iy <= end_y; ++iy)
	{
	  if (tile[ix][iy].surface == 0)
	    {
	      CL_Canvas* canvas;
	      //std::cout << "PingusSpotMap: Drawing to an emtpy tile: " << ix << " " << iy << std::endl;
	      canvas = new CL_Canvas(tile_size, tile_size);
	      Blitter::clear_canvas(canvas);

	      Blitter::put_surface(canvas, sprovider,
				   x - (ix * tile_size), y - (iy * tile_size));

	      tile[ix][iy].surface = CL_Surface (canvas, true);
	    }
	  else
	    {
	      Blitter::put_surface(static_cast<CL_Canvas*>(tile[ix][iy].surface.get_provider()),
				   sprovider,
				   x - (ix * tile_size), y - (iy * tile_size));
	    }
	  /*
	    CL_Surface s = CL_Surface::create(sprovider);
	  s->put_target(x - (ix * tile_size), y - (iy * tile_size), 0,
			tile[ix][iy].surface->get_provider());*/
	  tile[ix][iy].reload();
	  tile[ix][iy].set_empty(false);
	}
    }
}

// Generates a ColMap (with new) and returns a pointer to it
ColMap*
PingusSpotMap::get_colmap(void)
{
  return colmap;
}

/* EOF */
