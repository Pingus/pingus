//  $Id: smallmap_image.cxx,v 1.13 2003/10/21 21:37:06 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#include <ClanLib/Core/IOData/cl_endian.h>
#include <ClanLib/Display/pixel_buffer.h>
#include <ClanLib/Display/pixel_format.h>
#include "world.hxx"
#include "smallmap_image.hxx"
#include "col_map.hxx"
#include "server.hxx"

namespace Pingus {

SmallMapImage::SmallMapImage(Server* s, int width, int height)
  : server(s),
    canvas(width, height, width*4, CL_PixelFormat::rgba8888),
    sur(canvas),
    update_count(0),
    colmap_serial(0)
{
  update_surface();
}

void
SmallMapImage::update (float delta)
{
  float smallmap_update_time = 1.0f;

  update_count += delta;

  if (update_count > smallmap_update_time)
    {
      update_count = 0.0f;
      ColMap* colmap = server->get_world()->get_colmap();

      if (colmap_serial != colmap->get_serial())
        {
          update_surface();
        }
    }
}

CL_Surface
SmallMapImage::get_surface ()
{
  if (sur)
    {
      return sur;
    }
  else
    {
      update_surface();
      return sur;
    }
}

void
SmallMapImage::update_surface()
{
  unsigned char* buffer;
  unsigned char* cbuffer;
  unsigned char  current_pixel;

  ColMap* colmap = server->get_world()->get_colmap();
  buffer = colmap->get_data();

  colmap_serial = colmap->get_serial();

  canvas.lock();

  cbuffer = static_cast<unsigned char*>(canvas.get_data());

  int alpha = 255;

  int cmap_width  = colmap->get_width();
  int cmap_height = colmap->get_height();

  int width  = canvas.get_width();
  int height = canvas.get_height();
  for(int y = 0; y < height; ++y)
    {
      for (int x = 0; x < width; ++x)
	{
          // Index on the smallmap canvas
          int i = 4 * ((y * width) + x);

	  int tx = x * cmap_width / width;
	  int ty = y * cmap_height / height;

	  current_pixel = buffer[tx + (ty * cmap_width)];

	  switch (current_pixel)
            {
            case Groundtype::GP_NOTHING:
              if (!CL_Endian::is_system_big())
              {
                cbuffer[i + 0] = alpha;
                cbuffer[i + 1] = 0;
                cbuffer[i + 2] = 0;
                cbuffer[i + 3] = 0;
              }
              else
              {
                cbuffer[i + 3] = alpha;
                cbuffer[i + 2] = 0;
                cbuffer[i + 1] = 0;
                cbuffer[i + 0] = 0;
              }
              break;

            case Groundtype::GP_BRIDGE:
              if (!CL_Endian::is_system_big())
              {
                cbuffer[i + 0] = 255;
                cbuffer[i + 1] = 100;
                cbuffer[i + 2] = 255;
                cbuffer[i + 3] =   0;
              }
              else
              {
                cbuffer[i + 3] = 255;
                cbuffer[i + 2] = 100;
                cbuffer[i + 1] = 255;
                cbuffer[i + 0] =   0;
              }
              break;

            case Groundtype::GP_WATER:
            case Groundtype::GP_LAVA:
              if (!CL_Endian::is_system_big())
              {
                cbuffer[i + 0] = 255;
                cbuffer[i + 1] = 200;
                cbuffer[i + 2] = 0;
                cbuffer[i + 3] = 0;
              }
              else
              {
                cbuffer[i + 3] = 255;
                cbuffer[i + 2] = 200;
                cbuffer[i + 1] = 0;
                cbuffer[i + 0] = 0;
              }
              break;

#if 0
              // FIXME: temporaty disabled for 0.6.0 release, since all liquids are currently lava
            case Groundtype::GP_LAVA:
              if (!CL_Endian::is_system_big())
              {
                cbuffer[i + 0] = 255; // alpha
                cbuffer[i + 1] = 255; // blue
                cbuffer[i + 2] = 128;   // green
                cbuffer[i + 3] = 128;   // red
              }
              else
              {
                cbuffer[i + 3] = 255; // alpha
                cbuffer[i + 2] = 255; // blue
                cbuffer[i + 1] = 128;   // green
                cbuffer[i + 0] = 128;   // red
              }
              break;
#endif

            case Groundtype::GP_SOLID:
              if (!CL_Endian::is_system_big())
              {
                cbuffer[i + 0] = 255;
                cbuffer[i + 1] = 100;
                cbuffer[i + 2] = 100;
                cbuffer[i + 3] = 100;
              }
              else
              {
                cbuffer[i + 3] = 255;
                cbuffer[i + 2] = 100;
                cbuffer[i + 1] = 100;
                cbuffer[i + 0] = 100;
              }
              break;

            default:
              if (!CL_Endian::is_system_big())
              {
                cbuffer[i + 0] = 255;
                cbuffer[i + 1] = 200;
                cbuffer[i + 2] = 200;
                cbuffer[i + 3] = 200;
              }
              else
              {
                cbuffer[i + 3] = 255;
                cbuffer[i + 2] = 200;
                cbuffer[i + 1] = 200;
                cbuffer[i + 0] = 200;
              }
              break;
            }
	}
    }
  canvas.unlock();

  sur.set_pixeldata(CL_Point(0, 0), canvas);
}

} // namespace Pingus

/* EOF */
