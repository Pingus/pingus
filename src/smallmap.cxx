//  $Id: smallmap.cxx,v 1.3 2002/06/17 15:45:26 torangan Exp $
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

#include <ClanLib/Display/Display/display.h>
#include <ClanLib/Display/SurfaceProviders/canvas.h>
#include <ClanLib/Display/Input/mouse.h>

#include "pingu_holder.hxx"
#include "display.hxx"
#include "playfield.hxx"
#include "world.hxx"
#include "pingus_resource.hxx"
#include "smallmap.hxx"
#include "col_map.hxx"
#include "server.hxx"
#include "pingu.hxx"

using namespace std;

SmallMap::SmallMap()
{
  width = 175;
  height = 75;
  scroll_mode = false;
}

SmallMap::~SmallMap()
{
}
  
void
SmallMap::init()
{
  CL_Canvas*  canvas;
  unsigned char* buffer;
  unsigned char* cbuffer;
  unsigned char  current_pixel;
  int tx, ty;

  entrance_sur = PingusResource::load_surface("misc/smallmap_entrance", "core");
  exit_sur     = PingusResource::load_surface("misc/smallmap_exit", "core");

  ColMap* colmap = client->get_server()->get_world()->get_colmap(); 
  buffer = colmap->get_data();
  //Plf* plf = world->get_plf();

  canvas = new CL_Canvas(width, height);
 
  canvas->lock();
  
  cbuffer = static_cast<unsigned char*>(canvas->get_data());

  for(int y = 0; y < height; y++)
    {
      for (int x = 0; x < width; x++)
	{
	  tx = x * colmap->get_width() / width;
	  ty = y * colmap->get_height() / height;
	  
	  current_pixel = buffer[tx + (ty * colmap->get_width())];
	  
	  if (current_pixel == ColMap::NOTHING)
	    {
	      cbuffer[4 * ((y * width) + x) + 0] = 150;
	      cbuffer[4 * ((y * width) + x) + 1] = 0;
	      cbuffer[4 * ((y * width) + x) + 2] = 0;
	      cbuffer[4 * ((y * width) + x) + 3] = 0;
	    }
	  else if (current_pixel & ColMap::BRIDGE)
	    {
	      cbuffer[4 * ((y * width) + x) + 0] = 255;
	      cbuffer[4 * ((y * width) + x) + 1] = 100;
	      cbuffer[4 * ((y * width) + x) + 2] = 255;
	      cbuffer[4 * ((y * width) + x) + 3] =   0;
	    }
	  else if (current_pixel & ColMap::LAVA)
	    {
	      cbuffer[4 * ((y * width) + x) + 0] = 255;
	      cbuffer[4 * ((y * width) + x) + 1] = 100;
	      cbuffer[4 * ((y * width) + x) + 2] = 100;
	      cbuffer[4 * ((y * width) + x) + 3] = 255;
	    }
	  else if (current_pixel & ColMap::SOLID)
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
  /* FIXME: due to API change in PLF disabled
  std::vector<ExitData>     exit_d     = plf->get_exit();
  for(std::vector<ExitData>::iterator i = exit_d.begin(); i != exit_d.end(); i++)
    {
      // FIXME: Replace this with put_target() when it is bug free
      Blitter::put_surface(canvas, exit_sur, 
			   i->pos.x * width / colmap->get_width() - (exit_sur.get_width()/2), 
			   i->pos.y * height / colmap->get_height());
    }

  std::vector<EntranceData>     entrance_d     = plf->get_entrance();
  for(std::vector<EntranceData>::iterator i = entrance_d.begin(); i != entrance_d.end(); i++)
    {
      Blitter::put_surface(canvas, entrance_sur,
			   i->pos.x * width / colmap->get_width() - (entrance_sur.get_width()/2),
			   i->pos.y * height / colmap->get_height() - (entrance_sur.get_height()));
    }
  */
  canvas->unlock();
  
  sur = CL_Surface(canvas, true);
  
  x_pos = 0;
  y_pos = CL_Display::get_height() - sur.get_height();

  rwidth = CL_Display::get_width() * width / client->get_server()->get_world()->get_colmap()->get_width();
  rheight = CL_Display::get_height() * height / client->get_server()->get_world()->get_colmap()->get_height();
}

void
SmallMap::set_client(Client* c)
{
  client = c;
  init();
}

void
SmallMap::draw()
{
  Playfield* playfield = client->get_playfield();

  int x_of = playfield->get_x_offset();
  int y_of = playfield->get_y_offset();

  sur.put_screen(0, CL_Display::get_height() - sur.get_height()); 
  
  x_of = -x_of * width / client->get_server()->get_world()->get_colmap()->get_width();
  y_of = -y_of * height / client->get_server()->get_world()->get_colmap()->get_height();

  Display::draw_rect(x_of, 
		     y_of + CL_Display::get_height() - sur.get_height(),
		     x_of + min(rwidth, int(sur.get_width())),
		     y_of + min(rheight, int(sur.get_height())) + CL_Display::get_height() - sur.get_height(),
		     0.0, 1.0, 0.0, 1.0);
  
  // FIXME: This should use put_target(), but put_target(), does not
  // seem to work?!
  /*  vector<exit_data>     exit_d     = plf->get_exit();
  for(std::vector<exit_data>::iterator i = exit_d.begin(); i != exit_d.end(); i++)
    {
      exit_sur->put_screen(i->x_pos * width / colmap->get_width() +  x_pos - 3, 
			   i->y_pos * height / colmap->get_height() + y_pos - 3);
    }

  vector<entrance_data>     entrance_d     = plf->get_entrance();
  for(std::vector<entrance_data>::iterator i = entrance_d.begin(); i != entrance_d.end(); i++)
    {
      entrance_sur->put_screen(i->x_pos * width / colmap->get_width() + x_pos - 3,
			       i->y_pos * height / colmap->get_height() + y_pos);
    }
  */
  draw_pingus();
}

void
SmallMap::draw_pingus()
{
  int x;
  int y;
  PinguHolder* pingus = client->get_server()->get_world()->get_pingu_p();

  for(PinguIter i = pingus->begin(); i != pingus->end(); i++)
    {
      //FIXME: Replace this with put pixel
      x = x_pos + ((*i)->get_x() * width / client->get_server()->get_world()->get_colmap()->get_width());
      y = y_pos + ((*i)->get_y() * height / client->get_server()->get_world()->get_colmap()->get_height());

      CL_Display::draw_line(x, y, x, y-1, 1.0, 1.0, 0.0, 1.0);
    }
}

void
SmallMap::update(float /*delta*/)
{
  int cx, cy;
  ColMap* colmap = client->get_server()->get_world()->get_colmap();
  
  if (scroll_mode)
    {
      cx = (int(CL_Mouse::get_x()) - x_pos) * int(colmap->get_width()) / width;
      cy = (int(CL_Mouse::get_y()) - y_pos) * int(colmap->get_height()) / height ;

      client->get_playfield()->set_viewpoint(cx, cy);
    }
}

bool
SmallMap::mouse_over()
{
  return (CL_Mouse::get_x() > x_pos && CL_Mouse::get_x() < x_pos + (int)width
	  && CL_Mouse::get_y() > y_pos && CL_Mouse::get_y() < y_pos + (int)height);
}

bool
SmallMap::on_button_press(const CL_Key& key)
{
  switch(key.id)
    {
    case CL_MOUSE_LEFTBUTTON:
      if (mouse_over())
	{
	  scroll_mode = true;
	  return true;
	}
      break;
    }
  return false;
}

bool
SmallMap::on_button_release(const CL_Key& key)
{
  switch(key.id)
    {
    case CL_MOUSE_LEFTBUTTON:
      scroll_mode = false;
      break;
    }
  return false;
}

/* EOF */

