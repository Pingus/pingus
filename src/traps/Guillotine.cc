//  $Id: Guillotine.cc,v 1.1 2000/02/04 23:45:19 mbn Exp $
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

#include "../PingusResource.hh"
#include "Guillotine.hh"

Guillotine::Guillotine(trap_data data)
{
  killing = false;
  x_pos = data.x_pos;
  y_pos = data.y_pos;
  z_pos = data.z_pos;

  surface = CL_Surface::load("Traps/guillotinekill", PingusResource::get("traps.dat"));
  idle_surf = CL_Surface::load("Traps/guillotineidle", PingusResource::get("traps.dat"));

  counter.set_size(surface->get_num_frames()/2);
  counter.set_type(GameCounter::once);
  counter.set_speed(0.7);
  counter = 0;

  idle_counter.set_size(idle_surf->get_num_frames());
  idle_counter.set_type(GameCounter::loop);
  idle_counter.set_speed(0.3);
  idle_counter = 0;
}

Guillotine::~Guillotine()
{
}

void
Guillotine::draw_offset(int x, int y, float s)
{
  if (killing) {
    if (direction.is_left())
      surface->put_screen(x_pos + x, y_pos + y, counter);
    else
      surface->put_screen(x_pos + x, y_pos + y, counter + 12);
  } else {
    idle_surf->put_screen(x_pos + x, y_pos + y, idle_counter);
  }
}

void
Guillotine::let_move(void)
{
  if (counter.finished()) {
    counter = 0;
    killing = false;
  }

  if (killing) {
    ++counter;
  } else {
    ++idle_counter;
  }
}

bool
Guillotine::catch_pingu(Pingu* pingu)
{
  if (!killing) {
    if (pingu->x_pos < x_pos + 42 && pingu->x_pos > x_pos + 38 
	&& pingu->y_pos < y_pos + 98 && pingu->y_pos > y_pos + 94) {
      killing = true;
      pingu->set_status(dead);
      direction = pingu->direction;
    }
    return true;
  }
  return false;
}

/* EOF */
