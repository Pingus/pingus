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

#include <ClanLib/Display/pixel_buffer.h>
#include <ClanLib/Display/pixel_format.h>
#include "world.hxx"
#include "smallmap_image.hxx"
#include "col_map.hxx"
#include "server.hxx"

namespace Pingus {

SmallMapImage::SmallMapImage (Server * s, Vector arg_pos, int width, int height)
  : sur (create_surface (s, width, height)),
    pos (arg_pos)
{
}

SmallMapImage::~SmallMapImage ()
{
}

void
SmallMapImage::draw ()
{
  sur.draw (int(pos.x), int(pos.y));
}

void
SmallMapImage::update (float delta)
{
  // FIXME: Updating not implemented!
  UNUSED_ARG(delta);
}

CL_Surface
SmallMapImage::create_surface (Server * server, int width, int height)
{
  CL_PixelBuffer* canvas;
  unsigned char* buffer;
  unsigned char* cbuffer;
  unsigned char  current_pixel;
  int tx, ty;

  World* world = server->get_world();

  ColMap* colmap = world->get_colmap();
  buffer = colmap->get_data();

  canvas = new CL_PixelBuffer(width, height, width*4, CL_PixelFormat::rgba8888);

  canvas->lock();

  cbuffer = static_cast<unsigned char*>(canvas->get_data());

  for(int y = 0; y < height; y++)
    {
      for (int x = 0; x < width; x++)
	{
	  tx = x * colmap->get_width() / width;
	  ty = y * colmap->get_height() / height;

	  current_pixel = buffer[tx + (ty * colmap->get_width())];

	  if (current_pixel == Groundtype::GP_NOTHING)
	    {
	      cbuffer[4 * ((y * width) + x) + 0] = 150;
	      cbuffer[4 * ((y * width) + x) + 1] = 0;
	      cbuffer[4 * ((y * width) + x) + 2] = 0;
	      cbuffer[4 * ((y * width) + x) + 3] = 0;
	    }
	  else if (current_pixel == Groundtype::GP_BRIDGE)
	    {
	      cbuffer[4 * ((y * width) + x) + 0] = 255;
	      cbuffer[4 * ((y * width) + x) + 1] = 100;
	      cbuffer[4 * ((y * width) + x) + 2] = 255;
	      cbuffer[4 * ((y * width) + x) + 3] =   0;
	    }
	  else if (current_pixel == Groundtype::GP_LAVA)
	    {
	      cbuffer[4 * ((y * width) + x) + 0] = 255;
	      cbuffer[4 * ((y * width) + x) + 1] = 100;
	      cbuffer[4 * ((y * width) + x) + 2] = 100;
	      cbuffer[4 * ((y * width) + x) + 3] = 255;
	    }
	  else if (current_pixel == Groundtype::GP_SOLID)
	    {
	      cbuffer[4 * ((y * width) + x) + 0] = 255;
	      cbuffer[4 * ((y * width) + x) + 1] = 100;
	      cbuffer[4 * ((y * width) + x) + 2] = 100;
	      cbuffer[4 * ((y * width) + x) + 3] = 100;
	    }
	  else
	    {
	      cbuffer[4 * ((y * width) + x) + 0] = 255;
	      cbuffer[4 * ((y * width) + x) + 1] = 200;
	      cbuffer[4 * ((y * width) + x) + 2] = 200;
	      cbuffer[4 * ((y * width) + x) + 3] = 200;
	    }
	}
    }

#if 0
  /* This draws the exits and entrances to the smallmap, due to the
     class reordering this is no longer working, a generic
     WorldObj::draw_smallmap (SmallMap* / CL_Canvas*) should help
     here. */
  CL_Surface entrance_sur = PingusResource::load_surface("misc/smallmap_entrance", "core");
  CL_Surface exit_sur     = PingusResource::load_surface("misc/smallmap_exit", "core");

  PLF* plf = world->get_plf();

  std::vector<ExitData>     exit_d     = plf->get_exit();
  for(std::vector<ExitData>::iterator i = exit_d.begin(); i != exit_d.end(); i++)
    {
      // FIXME: Replace this with put_target() when it is bug free
      Blitter::put_surface(canvas, exit_sur,
			   i->pos.x * width / colmap->get_width() - (exit_sur.get_width()/2),
			   i->pos.y * height / colmap->get_height() - (exit_sur.get_height()));
    }

  std::vector<EntranceData>     entrance_d     = plf->get_entrance();
  for(std::vector<EntranceData>::iterator i = entrance_d.begin(); i != entrance_d.end(); ++i)
    {
      Blitter::put_surface(canvas, entrance_sur,
			   i->pos.x * width / colmap->get_width() - (entrance_sur.get_width()/2),
			   i->pos.y * height / colmap->get_height() - (entrance_sur.get_height()));

			   //entrance_sur->put_target(i->x_pos * width / colmap->get_width(),
			   //i->y_pos * height / colmap->get_height(),
			   //0, canvas);
    }
#endif

  canvas->unlock();

  return CL_Surface(canvas, true);
}

bool
SmallMapImage::mouse_over (int x, int y)
{
  return (pos.x < x && pos.x + sur.get_width () > x
	  &&
	  pos.y < y && pos.y + sur.get_height () > y);
}

} // namespace Pingus

/* EOF */
