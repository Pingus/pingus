//  $Id: Liquid.cc,v 1.1 2000/02/04 23:45:18 mbn Exp $
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

#include "PingusResource.hh"

#include "Liquid.hh"

Liquid::Liquid(liquid_data data)
{
  x_pos = data.x_pos;
  y_pos = data.y_pos;
  z_pos = data.z_pos;
  width = data.width;
  speed = data.speed;
  sur = CL_Surface::load(data.desc.res_name.c_str(), PingusResource::get("global.dat"));

  counter.set_size(sur->get_num_frames());

  counter.set_speed(speed);
}

Liquid::~Liquid()
{
}

void
Liquid::draw_offset(int x_of, int y_of, float s)
{
  int x1 = x_pos + x_of;
  int x2 = x_pos + width + x_of;
  int y1 = y_pos + y_of;
  int y2 = y_pos + y_of + sur->get_height();

  if (x1 < 0) {
    x1 = 0;
    if (x2 < 0)
      x2 = 0;
  }

  if (y1 < 0) {
    y1 = 0;
    if (y2 < 0)
      y2 = 0;
  }

  if (x2 >= CL_Display::get_width()) {
    x2 = CL_Display::get_width() - 1;
    if (x1 >= CL_Display::get_width())
      x1 = x2;
  }

  if (y2 >= CL_Display::get_height()) {
    y2 = CL_Display::get_height();
    if (y1 >= CL_Display::get_height())
      y1 = y2;
  }

  if (s == 1.0) {
    CL_Display::push_clip_rect();
    CL_Display::set_clip_rect(CL_ClipRect(x1, y1, x2, y2));
    
    for(int x = x_pos; x <= x_pos + width; x += sur->get_width())
      sur->put_screen(x + x_of, y_pos + y_of, int(counter));

    CL_Display::pop_clip_rect();
  } else {
    /* Do nothing, since this is buggy
    for(int x = x_pos; x <= x_pos + width; x += sur->get_width())
      sur->put_screen(x + x_of, y_pos + y_of,
                      s, s, int(counter));
    */
  }
  ++counter;
}

/* EOF */
