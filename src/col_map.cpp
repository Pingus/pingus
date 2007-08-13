//  $Id: col_map.cxx,v 1.23 2003/10/21 21:37:06 grumbel Exp $
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
#include "SDL.h"
#include "display/drawing_context.hpp"
#include "globals.hpp"
#include "col_map.hpp"
#include "collision_mask.hpp"
#include "pixel_buffer.hpp"
#include "pingus_error.hpp"
#include "math.hpp"
#include "gettext.h"
#include "sprite.hpp"

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

int
ColMap::getpixel_fast(int x, int y)
{
  return colmap[x+y*width];  
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
ColMap::remove(const CollisionMask& mask, int x, int y)
{
  ++serial;

  int swidth  = mask.get_width();
  int spitch  = mask.get_pitch();
  int sheight = mask.get_height();
  int y_offset = Math::max(-y, 0);
  int x_offset = Math::max(-x, 0);
  uint8_t* buffer = mask.get_data();

  for (int line = y_offset; line < sheight && (line + y) < height; ++line)
    {
      for (int i = x_offset; i < swidth && (i+x) < width; ++i)
        {
          if (buffer[i + (spitch*line)])
            {
              if (colmap[i + (width*(line+y) + x)] != Groundtype::GP_SOLID)
                colmap[i + (width*(line+y) + x)] = Groundtype::GP_NOTHING;
            }
        }
    }
}

void
ColMap::put(int x, int y, Groundtype::GPType p)
{
  ++serial; // FIXME: Shouldn't be here but at a more heigher level function

  if (x >= 0 && x < width
      && y >= 0 && y < height)
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

// Puts a surface on the colmap
void
ColMap::put(const CollisionMask& mask, int sur_x, int sur_y, Groundtype::GPType pixel)
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

  // FIXME: This could be speed up quite a bit
  uint8_t* source = mask.get_data();
  for (int y = 0; y < mask.get_height(); ++y)
    for (int x = 0; x < mask.get_width(); ++x)
      {
        if (source[y * mask.get_width() + x])
          if (blit_allowed(x + sur_x, y + sur_y, pixel))
            put(x + sur_x, y + sur_y, pixel);
      }
}

void
ColMap::draw(DrawingContext& gc)
{
  PixelBuffer canvas(width, height);
  unsigned char* buffer;

  canvas.lock();
  buffer = static_cast<unsigned char*>(canvas.get_data());

  for(int i = 0; i < (width * height); ++i)
    {
      switch(colmap[i])
	{
	case Groundtype::GP_NOTHING:
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	  buffer[i * 4 + 0] = 0;
	  buffer[i * 4 + 1] = 0;
	  buffer[i * 4 + 2] = 0;
	  buffer[i * 4 + 3] = 0;
#else
	  buffer[i * 4 + 3] = 0;
	  buffer[i * 4 + 2] = 0;
	  buffer[i * 4 + 1] = 0;
	  buffer[i * 4 + 30] = 0;
#endif
	  break;

	case Groundtype::GP_SOLID:
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	  buffer[i * 4 + 0] = 255;
	  buffer[i * 4 + 1] = 100;
	  buffer[i * 4 + 2] = 100;
	  buffer[i * 4 + 3] = 100;
#else
	  buffer[i * 4 + 3] = 255;
	  buffer[i * 4 + 2] = 100;
	  buffer[i * 4 + 1] = 100;
	  buffer[i * 4 + 30] = 100;
#endif
	  break;

	case Groundtype::GP_BRIDGE:
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	  buffer[i * 4 + 0] = 255;
	  buffer[i * 4 + 1] = 0;
	  buffer[i * 4 + 2] = 0;
	  buffer[i * 4 + 3] = 200;
#else
	  buffer[i * 4 + 3] = 255;
	  buffer[i * 4 + 2] = 0;
	  buffer[i * 4 + 1] = 0;
	  buffer[i * 4 + 0] = 200;
#endif
	  break;

	default:
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	  buffer[i * 4 + 0] = 255;
	  buffer[i * 4 + 1] = 200;
	  buffer[i * 4 + 2] = 200;
	  buffer[i * 4 + 3] = 200;
#else
	  buffer[i * 4 + 3] = 255;
	  buffer[i * 4 + 2] = 200;
	  buffer[i * 4 + 1] = 200;
	  buffer[i * 4 + 0] = 200;
#endif
	  break;
	}
    }

  canvas.unlock();

  Sprite sprite(canvas);
  gc.draw(sprite, 0, 0);
}

unsigned
ColMap::get_serial()
{
  return serial;
}

/* EOF */
