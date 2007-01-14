//  $Id: smallmap.cxx,v 1.41 2003/10/21 21:37:06 grumbel Exp $
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

#include <ClanLib/Display/display.h>
#include "../pingu_holder.hxx"
#include "../gui/display.hxx"
#include "../display/drawing_context.hxx"
#include "../world.hxx"
#include "../resource.hxx"
#include "../col_map.hxx"
#include "../true_server.hxx"
#include "../smallmap_image.hxx"
#include "../pingu.hxx"
#include "../math.hxx"
#include "../vector.hxx"
#include "../globals.hxx"
#include "playfield.hxx"
#include "smallmap.hxx"


SmallMap::SmallMap(Client* c)
  : client(c),
    gc_ptr(0)
{
  int max_width = 175;
  int max_height = 100;

  int min_height = 70;
  int min_width = 100;

  ColMap* colmap = client->get_server()->get_world()->get_colmap();

  // Scaling values used in order to keep the aspect ratio
  int x_scaling = colmap->get_width()  / max_width;
  int y_scaling = colmap->get_height() / max_height;

  // If at best one horizontal pixel in the smallmap represents more colmap
  // pixels than one vertical pixel
  if (x_scaling > y_scaling)
    {
      width  = max_width;
      height = Math::max(min_height, colmap->get_height() / x_scaling);
    }
  else
    {
      width  = Math::max(min_width, colmap->get_width() / y_scaling);
      height = max_height;
    }
  
  x_pos   = 5;
  y_pos   = CL_Display::get_height() - height - 5;
  rwidth  = CL_Display::get_width() * width / client->get_server()->get_world()->get_colmap()->get_width();
  rheight = CL_Display::get_height() * height / client->get_server()->get_world()->get_colmap()->get_height();

  image = new SmallMapImage(c->get_server(), width, height);

  scroll_mode = false;
}

SmallMap::~SmallMap()
{
  delete image;
}

void
SmallMap::draw (DrawingContext& gc)
{
  // FIXME: This is potentially dangerous, since we don't know how
  // long 'gc' will be alive. Should use a DrawingContext for caching.
  gc_ptr = &gc;

  Playfield* playfield = client->get_playfield();

  gc.draw(image->get_surface(), Vector((float)x_pos, (float)y_pos));

  CL_Point of = playfield->get_pos();
    
  of.x = x_pos + of.x * width  / client->get_server()->get_world()->get_colmap()->get_width();
  of.y = y_pos + of.y * height / client->get_server()->get_world()->get_colmap()->get_height();

  int w = Math::min(rwidth,  int(width  - 1));
  int h = Math::min(rheight, int(height - 1));

  gc.draw_rect(float(of.x - w/2), float(of.y - h/2),
               float(of.x + w/2), float(of.y + h/2),
               CL_Color(0, 255, 0));

  client->get_server()->get_world()->draw_smallmap(this);

  World* const& world = client->get_server()->get_world();
  PinguHolder* pingus = world->get_pingus();

  for(PinguIter i = pingus->begin(); i != pingus->end(); ++i)
    {
      int x = static_cast<int>(x_pos + ((*i)->get_x() * width  / world->get_colmap()->get_width()));
      int y = static_cast<int>(y_pos + ((*i)->get_y() * height / world->get_colmap()->get_height()));

      gc.draw_line((float)x, (float)y, (float)x, (float)y-2, CL_Color(255, 255, 0));
    }

  gc_ptr = 0;
}

void
SmallMap::update (float delta)
{
  image->update(delta);
}

void
SmallMap::draw_sprite(CL_Sprite sprite, Vector pos)
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
