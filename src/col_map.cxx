//  $Id: col_map.cxx,v 1.14 2002/12/29 23:29:00 torangan Exp $
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

#include <iostream>
#include <ClanLib/Display/SurfaceProviders/canvas.h>
#include "graphic_context.hxx"
#include "globals.hxx"
#include "col_map.hxx"

/* Headers needed for i18n / gettext */
#include <clocale>
#include <config.h>
#include "my_gettext.hxx"

#define COLMAP_WITH_MEMORY_HOLE 1

// Obtain the colmap from a memory area
ColMap::ColMap(int w, int h)
  : serial(0),
    width(w),
    height(h),
    colmap(new unsigned char[width * height])
{
  // Clear the colmap
  memset(colmap, Groundtype::GP_NOTHING, sizeof(unsigned char) * width * height);
}

ColMap::~ColMap()
{
  //std::cout << "ColMap:~ColMap" << std::endl;
  delete[] colmap;
}

int
ColMap::getpixel(int x, int y)
{
  if (x >= 0 && x < width && y >= 0 && y < height) {
    return colmap[x+y*width];
  } else {
    return Groundtype::GP_OUTOFSCREEN; 
  }
}

unsigned char* 
ColMap::get_data()
{
  return colmap;
}

int
ColMap::get_height()
{
  return height;
}

int
ColMap::get_width()
{
  return width;
}

void 
ColMap::remove(const CL_Surface& sur, int x, int y)
{
  remove(sur.get_provider(), x, y);
}

void 
ColMap::remove(CL_SurfaceProvider* provider, int x, int y)
{
  ++serial;

  assert(provider);

  if (provider->get_depth() == 32) 
    {
      unsigned char* buffer;
      int swidth = provider->get_width();
      int sheight = provider->get_height();
      int y_offset = -y;
      int x_offset = -x;
      if (y_offset < 0) y_offset = 0;
      if (x_offset < 0) x_offset = 0;

      provider->lock();
      buffer = static_cast<unsigned char*>(provider->get_data());

      for(int line = y_offset; line < sheight && (line + y) < height; ++line) 
	{
	  for (int i = x_offset; i < swidth && (i+x) < width; ++i) 
	    {
	      if (buffer[(i + (swidth*line)) * 4] != 0) 
		{
		  if (colmap[i + (width*(line+y) + x)] != Groundtype::GP_SOLID)
		    colmap[i + (width*(line+y) + x)] = Groundtype::GP_NOTHING;
		}
	    }
	}
#if COLMAP_WITH_MEMORY_HOLE
      provider->unlock();
#endif
    }
  else if (provider->get_depth() == 8)
    {
      unsigned char* buffer;
      int swidth = provider->get_width();
      int sheight = provider->get_height();
      int y_offset = -y;
      int x_offset = -x;
      if (y_offset < 0) y_offset = 0;
      if (x_offset < 0) x_offset = 0;

      provider->lock();
      buffer = static_cast<unsigned char*>(provider->get_data());

      for(int line = y_offset; line < sheight && (line + y) < height; ++line) 
	{
	  for (int i = x_offset; i < swidth && (i+x) < width; ++i) 
	    {
	      if (buffer[i + (swidth*line)]) 
		{
		  if (colmap[i + (width*(line+y) + x)] != Groundtype::GP_SOLID)
		    colmap[i + (width*(line+y) + x)] = Groundtype::GP_NOTHING;
		}
	    }
	}
#if COLMAP_WITH_MEMORY_HOLE
      provider->unlock();
#endif
    }
  else
    {
      assert(!"ColMap::remove: Color depth not supported");
    }
}

void
ColMap::put(int x, int y, Groundtype::GPType p)
{
  ++serial;

  if (x > 0 && x < width
      && y > 0 && y < height) 
    {
      colmap[x+y*width] = p;
    }
  else
    {
      if (verbose > 2)
	std::cout << "ColMap: remove: Out of map" << std::endl;
    }
}

bool
ColMap::blit_allowed (int x, int y,  Groundtype::GPType gtype)
{
  // FIXME: Inline me
  if (gtype == Groundtype::GP_BRIDGE)
    {
      int pixel = getpixel (x, y);
      return pixel == Groundtype::GP_NOTHING;
    }
  else
    {
      return true;
    }
}

void
ColMap::put(const CL_Surface& sur, int sur_x, int sur_y, Groundtype::GPType gptype)
{
  put(sur.get_provider(), sur_x, sur_y, gptype);
}

// Puts a surface on the colmap
void
ColMap::put(CL_SurfaceProvider* provider, int sur_x, int sur_y, Groundtype::GPType pixel)
{
  // transparent groundpieces are only drawn on the gfx map, not on the colmap
  if (pixel == Groundtype::GP_TRANSPARENT)
    return;

  if ((sur_x > width) || (sur_y > height)) 
    {
      if (verbose > 3) 
	{
	  std::cout << "Warning: ColMap: Spot out of screen" << std::endl;
	  std::cout << "sur_x: " << sur_x << " sur_y: " << sur_y << std::endl;
	}
      return;
    }

  provider->lock();
  
  if (provider->get_depth() == 32) 
    {
      float r, g, b, a;
      // Rewritting blitter for 32bit depth (using get_pixel())
      for (unsigned int y=0; y < provider->get_height(); ++y)
	for (unsigned int x=0; x < provider->get_width(); ++x) 
	  {
	    provider->get_pixel(x, y, &r, &g, &b, &a);
	    if (a > 0.1) // Alpha threshold
	      {
		if (blit_allowed (x + sur_x, y + sur_y, pixel))
		  put(x + sur_x, y + sur_y, pixel);
	      }
	  }
    }
  else if (provider->get_depth() == 8)
    {
      unsigned char* buffer;
      int swidth = provider->get_width();
      int sheight = provider->get_height();
      int y_offset = -sur_y;
      int x_offset = -sur_x;
      if (y_offset < 0) y_offset = 0;
      if (x_offset < 0) x_offset = 0;

      //provider->lock();
      buffer = static_cast<unsigned char*>(provider->get_data());

      if (provider->uses_src_colorkey())
	{
	  unsigned int colorkey = provider->get_src_colorkey();
	  for(int line = y_offset; line < sheight && (line + sur_y) < height; ++line) 
	    for (int i = x_offset; i < swidth && (i+sur_x) < width; ++i) 
	      {
		if (buffer[i + (swidth*line)] != colorkey)
		  {
		    if (blit_allowed (i + sur_x, line + sur_y, pixel))
		      colmap[i + (width*(line+sur_y) + sur_x)] = pixel;		    
		  }
	      }
	}
      else
	{
	  for(int line = y_offset; line < sheight && (line + sur_y) < height; ++line) 
	    for (int i = x_offset; i < swidth && (i+sur_x) < width; ++i) 
	      {
		if (blit_allowed (i + sur_x, line + sur_y, pixel))
		  colmap[i + (width*(line+sur_y) + sur_x)] = pixel;
	      }
	}
    }
  else
    {
      std::cout << "ColMap: Unsupported color depth, ignoring" << std::endl;
    }
  // FIXME: Memory hole
  // provider->unlock();
}

void
ColMap::draw(GraphicContext& gc)
{
  CL_Canvas* canvas = new CL_Canvas(width, height);
  CL_Surface sur;
  unsigned char* buffer;

  canvas->lock();
  buffer = static_cast<unsigned char*>(canvas->get_data());
  
  for(int i = 0; i < (width * height); ++i)
    {
      switch(colmap[i])
	{
	case Groundtype::GP_NOTHING:
	  buffer[i * 4 + 0] = 0;
	  buffer[i * 4 + 1] = 0;
	  buffer[i * 4 + 2] = 0;
	  buffer[i * 4 + 3] = 0;
	  break;

	case Groundtype::GP_SOLID:
	  buffer[i * 4 + 0] = 255;
	  buffer[i * 4 + 1] = 100;
	  buffer[i * 4 + 2] = 100;
	  buffer[i * 4 + 3] = 100;
	  break;

	case Groundtype::GP_BRIDGE:
	  buffer[i * 4 + 0] = 255;
	  buffer[i * 4 + 1] = 0;
	  buffer[i * 4 + 2] = 0;
	  buffer[i * 4 + 3] = 200;
	  break;

	default:
	  buffer[i * 4 + 0] = 255;
	  buffer[i * 4 + 1] = 200;
	  buffer[i * 4 + 2] = 200;
	  buffer[i * 4 + 3] = 200;
	  break;
	}
    }

  // FIXME: Memory hole
#if COLMAP_WITH_MEMORY_HOLE
  canvas->unlock();
#endif

  sur = CL_Surface(canvas, true);

  gc.draw(sur, 0, 0);
}

unsigned
ColMap::get_serial()
{
  return serial;
}

/* EOF */
