//  $Id: smallmap.cxx,v 1.26 2002/10/17 00:10:46 grumbel Exp $
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
#include "pingu_holder.hxx"
#include "display.hxx"
#include "playfield.hxx"
#include "world.hxx"
#include "pingus_resource.hxx"
#include "smallmap.hxx"
#include "col_map.hxx"
#include "true_server.hxx"
#include "pingu.hxx"
#include "timer.hxx"
#include "math.hxx"
#include "globals.hxx"

using namespace std;

SmallMap::SmallMap()
  : canvas(0),
    update_count(0),
    gc_ptr(0)
{
  max_width = 175;
  max_height = 100;

  // Don't really need to initialise these now as this values are calculated
  // later on.  However, initialise here, just in case.
  width = 175;
  height = 100;

  scroll_mode = false;
}

SmallMap::~SmallMap()
{
  delete canvas;
}
  
void
SmallMap::init()
{
  unsigned char* buffer;
  unsigned char* cbuffer;
  unsigned char  current_pixel;
  int tx, ty;

  entrance_sur = PingusResource::load_surface("misc/smallmap_entrance", "core");
  exit_sur     = PingusResource::load_surface("misc/smallmap_exit", "core");

  ColMap* colmap = client->get_server()->get_world()->get_colmap(); 
  buffer = colmap->get_data();

  colmap_serial = colmap->get_serial();

  if (!canvas)
    {
      // Scaling values used in order to keep the aspect ratio
      int x_scaling = colmap->get_width() / max_width;
      int y_scaling = colmap->get_height() / max_height;

      // If at best one horizontal pixel in the smallmap represents more colmap
      // pixels than one vertical pixel
      if (x_scaling > y_scaling)
        {
          width = max_width;
          height = colmap->get_height() / x_scaling;
        }
      else
        {
          width = colmap->get_width() / y_scaling;
          height = max_height;
        }

      canvas = new CL_Canvas(width, height);
    }
 
  canvas->lock();
  
  cbuffer = static_cast<unsigned char*>(canvas->get_data());

  int alpha;
  if (fast_mode)
    alpha = 255;
  else
    alpha = 150;

  int cmap_width  = colmap->get_width();
  int cmap_height = colmap->get_height();

  for(int y = 0; y < height; ++y)
    {
      for (int x = 0; x < width; ++x)
	{
          // Index on the smallmap canvas
          int i = 4 * ((y * width) + x);

	  tx = x * cmap_width / width;
	  ty = y * cmap_height / height;
	  
	  current_pixel = buffer[tx + (ty * cmap_width)];
	  
	  switch (current_pixel)
            {
            case Groundtype::GP_NOTHING:
	      cbuffer[i + 0] = alpha;
              cbuffer[i + 1] = 0;
	      cbuffer[i + 2] = 0;
	      cbuffer[i + 3] = 0;
              break;
              
            case Groundtype::GP_BRIDGE:
              cbuffer[i + 0] = 255;
	      cbuffer[i + 1] = 100;
	      cbuffer[i + 2] = 255;
	      cbuffer[i + 3] =   0;
              break;
              
            case Groundtype::GP_LAVA:
              cbuffer[i + 0] = 255;
	      cbuffer[i + 1] = 100;
	      cbuffer[i + 2] = 100;
	      cbuffer[i + 3] = 255;
              break;
              
            case Groundtype::GP_SOLID:
              cbuffer[i + 0] = 255;
	      cbuffer[i + 1] = 100;
	      cbuffer[i + 2] = 100;
	      cbuffer[i + 3] = 100;
              break;

            default:
              cbuffer[i + 0] = 255;
	      cbuffer[i + 1] = 200;
	      cbuffer[i + 2] = 200;
	      cbuffer[i + 3] = 200;
              break;
            }
	}
    }
  canvas->unlock();

  //Timer surface_timer("Smallmap surface creation");
  sur = CL_Surface(canvas, false);
  //surface_timer.stop();

  x_pos = 5;
  y_pos = CL_Display::get_height() - sur.get_height();

  rwidth = CL_Display::get_width() * width / client->get_server()->get_world()->get_colmap()->get_width();
  rheight = CL_Display::get_height() * height / client->get_server()->get_world()->get_colmap()->get_height();
}

void
SmallMap::set_client (Client* c)
{
  client = c;
  init();
}

void
SmallMap::draw (GraphicContext& gc)
{
  gc_ptr = &gc;

  Playfield* playfield = client->get_playfield();

  int x_of = playfield->get_x_offset();
  int y_of = playfield->get_y_offset();

  sur.put_screen(x_pos, y_pos); 

  if (has_focus)
    Display::draw_rect(x_pos, y_pos,
		       x_pos + sur.get_width (), y_pos + sur.get_height () - 1, 
		       1.0f, 1.0f, 1.0f, 1.0f);
		       
  
  x_of = x_pos - x_of * width / client->get_server()->get_world()->get_colmap()->get_width();
  y_of = y_pos - y_of * height / client->get_server()->get_world()->get_colmap()->get_height();

  Display::draw_rect(x_of, 
		     y_of,
		     x_of + Math::min(rwidth,  static_cast<int>(sur.get_width())),
		     y_of + Math::min(rheight, static_cast<int>(sur.get_height())),
		     0.0, 1.0, 0.0, 1.0);
  
  // FIXME: This should use put_target(), but put_target(), does not
  // seem to work?!
  /*  vector<exit_data>     exit_d     = plf->get_exit();
  for(std::vector<exit_data>::iterator i = exit_d.begin(); i != exit_d.end(); ++i)
    {
      exit_sur->put_screen(i->x_pos * width / colmap->get_width() +  x_pos - 3, 
			   i->y_pos * height / colmap->get_height() + y_pos - 3);
    }

  vector<entrance_data>     entrance_d     = plf->get_entrance();
  for(std::vector<entrance_data>::iterator i = entrance_d.begin(); i != entrance_d.end(); ++i)
    {
      entrance_sur->put_screen(i->x_pos * width / colmap->get_width() + x_pos - 3,
			       i->y_pos * height / colmap->get_height() + y_pos);
    }
  */
  client->get_server()->get_world()->draw_smallmap(this);

  draw_pingus();

  gc_ptr = 0;
}

void
SmallMap::draw_pingus ()
{
  int x;
  int y;
  World* const& world = client->get_server()->get_world();
  PinguHolder* pingus = world->get_pingus();

  for(PinguIter i = pingus->begin(); i != pingus->end(); ++i)
    {
      x = static_cast<int>(x_pos + ((*i)->get_x() * width  / world->get_colmap()->get_width()));
      y = static_cast<int>(y_pos + ((*i)->get_y() * height / world->get_colmap()->get_height()));

      //FIXME: Replace this with draw_pixel (only availabe in 0.7)
      CL_Display::draw_line(x,   y, x,   y, 1.0, 1.0, 0.0, 1.0);
      CL_Display::draw_line(x, y-1, x, y-1, 1.0, 1.0, 0.0, 1.0);
      CL_Display::draw_line(x, y-2, x, y-2, 1.0, 1.0, 0.0, 1.0);
    }
}

void
SmallMap::update (float delta)
{
  float smallmap_update_time = 1.0f;

  update_count += delta;

  if (update_count > smallmap_update_time)
    {
      update_count = 0.0f;
      ColMap* colmap = client->get_server()->get_world()->get_colmap(); 

      if (colmap_serial != colmap->get_serial())
        {
          init();
        }
    }
}

void
SmallMap::draw_sprite(Sprite sprite, Vector pos)
{
  World* world = client->get_server()->get_world();
  float x = x_pos + (pos.x * width  / world->get_colmap()->get_width());
  float y = y_pos + (pos.y * height / world->get_colmap()->get_height());
  
  gc_ptr->draw(sprite, Vector(x, y));
}

bool
SmallMap::is_at (int x, int y)
{
  return (x > x_pos && x < x_pos + (int)width
	  && y > y_pos && y < y_pos + (int)height);
}

void
SmallMap::on_pointer_move (int x, int y)
{
  int cx, cy;
  ColMap* colmap = client->get_server()->get_world()->get_colmap();

  if (scroll_mode)
    {
      cx = (x - x_pos) * static_cast<int>(colmap->get_width()  / width);
      cy = (y - y_pos) * static_cast<int>(colmap->get_height() / height);
      
      client->get_playfield()->set_viewpoint(cx, cy);
    }
}

void
SmallMap::on_primary_button_press (int x, int y)
{
  scroll_mode = true;

  // set view to the given COs
  int cx, cy;
  ColMap* colmap = client->get_server()->get_world()->get_colmap();
  cx = (x - x_pos) * int(colmap->get_width()) / width;
  cy = (y - y_pos) * int(colmap->get_height()) / height ;
  client->get_playfield()->set_viewpoint(cx, cy);
}

void
SmallMap::on_primary_button_release(int x, int y)
{
  scroll_mode = false;
  UNUSED_ARG(x);
  UNUSED_ARG(y);
}

void
SmallMap::on_pointer_enter ()
{
  has_focus = true;  
}

void
SmallMap::on_pointer_leave ()
{
  has_focus = false;  
}

/* EOF */

