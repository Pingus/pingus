//  $Id: View.cc,v 1.5 2000/02/27 21:05:06 grumbel Exp $
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

#include <cassert>
#include <ClanLib/core.h>

#include "View.hh"
#include "globals.hh"

// static variables
World* View::world;

View::View(int x1, int y1, int x2, int y2, float s)
{
  assert(world);

  size = s;

  cout <<  "X1: " << x1 << " X2: " << x2 
       << " Y1: " << y1 << " Y2: " << y2 << endl;

  clip_rect = CL_ClipRect(x1, y1, x2, y2);

  x1_pos = x1;
  y1_pos = y1;

  x2_pos = x2;
  y2_pos = y2;
  
  x_offset = 0;
  y_offset = 0;

  current_pingu = 0;

  make_range();
}

void
View::draw()
{
  assert(world);
  float color;

  if (is_over(CL_Mouse::get_x(), CL_Mouse::get_y()))
    {
      color = 1.0;
      mouse_over = true;
    } 
  else 
    {
      color = 0.0;
      mouse_over = false;
    }

  CL_Display::push_clip_rect();
  CL_Display::set_clip_rect(clip_rect);

  // Drawing the world
  world->draw_offset(x_offset + x1_pos, y_offset + y1_pos, size);
  cap.set_pingu(current_pingu);
  cap.draw_offset(get_x_pos() + get_x_offset(), 
		  get_y_pos() + get_y_offset(),
		  size);
  
  /*  std::cout <<  "XPos: " << -get_x_pos() - get_x_offset() + CL_Mouse::get_x()
      << " YPos: " << -get_y_pos() - get_y_offset() + CL_Mouse::get_y()
      << " XOffset: " << get_x_offset()
      << " YOffset: " << get_y_offset()
      << std::endl;  */

  CL_Display::pop_clip_rect();  
}

View::~View()
{
}

void
View::set_world(World* w)
{
  world = w;
}

bool
View::is_over(int x, int y)
{
  if (x >= x1_pos && x <= x2_pos 
      && y >= y1_pos && y <= y2_pos) {
    return true;
  } else {
    return false;
  }
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
  x_offset.set_range(0, x2_pos - x1_pos - (world->get_width() * size));
  y_offset.set_range(0, y2_pos - y1_pos - (world->get_height() * size));
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

/* EOF */
