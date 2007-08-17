//  $Id$
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
ColMap::remove(const CollisionMask& mask, int x_pos, int y_pos)
{
  ++serial;

  int swidth  = mask.get_width();
  int sheight = mask.get_height();
  uint8_t* buffer = mask.get_data();

  int start_x = Math::max(0, -x_pos);
  int start_y = Math::max(0, -y_pos);
  int end_x   = Math::min(swidth,  width  - x_pos);
  int end_y   = Math::min(sheight, height - y_pos);

  for (int y = start_y; y < end_y; ++y)
    {
      for (int x = start_x; x < end_x; ++x)
        {
          if (buffer[y*swidth + x])
            {
              uint8_t& pixel = colmap[(y+y_pos)*width + (x+x_pos)];
              if (pixel != Groundtype::GP_SOLID)
                pixel = Groundtype::GP_NOTHING;
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
      int pixel = getpixel(x, y);
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

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  const int red   = 3;
  const int green = 2;
  const int blue  = 1;
  const int alpha = 0;  
#else
  const int red   = 0;
  const int green = 1;
  const int blue  = 2;
  const int alpha = 3;
#endif

  uint8_t trans = 220;

  for(int i = 0; i < (width * height); ++i)
    {
      switch(colmap[i])
	{
	case Groundtype::GP_NOTHING:
	  buffer[i * 4 + red  ] =   0;
	  buffer[i * 4 + green] =   0;
	  buffer[i * 4 + blue ] =   0;
	  buffer[i * 4 + alpha] =   0;
	  break;

	case Groundtype::GP_SOLID:
	  buffer[i * 4 + red  ] = 100;
	  buffer[i * 4 + green] = 100;
	  buffer[i * 4 + blue ] = 100;
	  buffer[i * 4 + alpha] = trans;
	  break;

	case Groundtype::GP_BRIDGE:
	  buffer[i * 4 + red  ] = 200;
	  buffer[i * 4 + green] =   0;
	  buffer[i * 4 + blue ] =   0;
	  buffer[i * 4 + alpha] = trans;
	  break;

	default:
	  buffer[i * 4 + red  ] = 200;
	  buffer[i * 4 + green] = 200;
	  buffer[i * 4 + blue ] = 200;
	  buffer[i * 4 + alpha] = trans;
	  break;
	}
    }

  canvas.unlock();

  Sprite sprite(canvas);
  gc.draw(sprite, 0, 0, 1000);
}

unsigned
ColMap::get_serial()
{
  return serial;
}

/* EOF */
