//  $Id: view.cxx,v 1.13 2002/09/28 11:52:22 torangan Exp $
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

#include <assert.h>
#include "view.hxx"
#include "world.hxx"
#include "client.hxx"

// static variables
World* View::world;

View::View(Client* client, int x1, int y1, int x2, int y2, float s)
  : display_gc (x1, y1, x2, y2, 0, 0),
    cap (client->get_button_panel ()),
    current_pingu (0)
{
  mouse_over = false;
  assert(world);

  size = s;
  
  // FIXME: clip_rect = CL_ClipRect(x1, y1, x2 + 1, y2 + 1);

  x1_pos = x1;
  y1_pos = y1;

  x2_pos = x2;
  y2_pos = y2;
  
  x_offset = 0;
  y_offset = 0;

  make_range();
}

View::~View()
{
}

void
View::draw()
{
  assert(world);
  
  // Update the scroll position
  //display_gc.set_zoom (2.0f);
  display_gc.set_offset (x_offset - (x2_pos - x1_pos)/2,
			 y_offset - (y2_pos - y1_pos)/2);
  world->draw (display_gc);
  
  cap.set_pingu(current_pingu);
  cap.draw_offset(get_x_pos() + get_x_offset(), 
		  get_y_pos() + get_y_offset(),
		  size);
  
    // FIXME: CL_Display::pop_clip_rect();
}

void
View::update (float /*delta */)
{
  if (is_over(mouse_x, mouse_y))
    {
      mouse_over = true;
    }
  else 
    {
      mouse_over = false;
    }
}

void
View::set_world(World* w)
{
  world = w;
}

bool
View::is_over(int x, int y)
{
  return (x >= x1_pos && x <= x2_pos && y >= y1_pos && y <= y2_pos);
}

bool
View::is_current()
{
  return mouse_over;
}

int
View::get_x_offset() const
{
  return (int)x_offset;
}

int
View::get_y_offset() const
{
  return (int)y_offset;
}

void
View::set_x_offset(int x)
{
  x_offset = x;
}

void
View::set_y_offset(int y)
{
  y_offset = y;
}

void
View::shift_x_offset(int dx)
{
  x_offset += dx;
}

void
View::shift_y_offset(int dy)
{
  y_offset += dy;
}

int
View::get_x_pos() const
{
  return x1_pos;
}

int
View::get_y_pos() const
{
  return y1_pos;
}

void
View::set_zoom(double s)
{
  size = s;
  make_range();
}

void
View::make_range()
{
  // Bug: this works not very good
  x_offset.set_range(0, (int)((x2_pos - x1_pos + 1) - (world->get_width()  * size)));
  y_offset.set_range(0, (int)((y2_pos - y1_pos + 1) - (world->get_height() * size)));
}

double
View::get_zoom(void)
{
  return size;
}

void 
View::set_pingu(Pingu* p)
{
  current_pingu = p;
}

Vector 
View::get_center ()
{
  return Vector (-x_offset + get_width ()/2, -y_offset + get_height ()/2);
}

void
View::on_pointer_move (int x, int y)
{
  mouse_x = x;
  mouse_y = y;
}

/* EOF */
