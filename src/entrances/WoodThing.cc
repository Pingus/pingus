//  $Id: WoodThing.cc,v 1.2 2000/02/09 21:43:43 grumbel Exp $
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

#include <cstdlib>

#include "../PingusResource.hh"
#include "../particles/SmokeParticle.hh"
#include "../particles/GroundParticle.hh"
#include "../algo.hh"

#include "WoodThing.hh"

WoodThing::WoodThing(entrance_data data)
{
  x_pos = data.x_pos;
  y_pos = data.y_pos;
  release_rate = data.release_rate;
  last_release = -release_rate;
  z_pos = -10;
  direction = data.direction;

  surface = CL_Surface::load("Entrances/woodthing_mov", PingusResource::get("global.dat"));
  surface2 = CL_Surface::load("Entrances/woodthing_nmov", PingusResource::get("global.dat"));

  counter.set_size(surface->get_num_frames());
  counter.set_type(GameCounter::once);
  counter.set_speed(2);
  counter =  surface->get_num_frames() - 1;
}

void
WoodThing::let_move(void)
{
  --counter;

  if (rand() % 5 == 0) {
    /*
    particle->add_particle(new GroundParticle(x_pos - (surface->get_width()/2) - 24,
					     y_pos - surface->get_height() + 32 - 147,
					     frand() * 4 - 4,
					     (frand()/ 2.0) * - 4.0));*/
    // This does not work and I have no idea why?!
    /*    particle->add_particle(new SmokeParticle(x_pos - (surface->get_width()/2) - 24,
					     y_pos - surface->get_height() + 32 - 147,
					     frand() * 4 - 4,
					     (frand()/ 2.0) * - 4.0));
   particle->add_particle(new SmokeParticle());*/
  }
}

void
WoodThing::draw_offset(int x, int y, float s)
{
  if (s == 1.0) {
    surface->put_screen(x_pos - (surface->get_width()/2) + x,
			y_pos - surface->get_height() + 32 + y,
			counter);
    //    surface2->put_screen(x_pos - (surface->get_width()/2) + x - 24,
     		 //			y_pos - surface->get_height() + 32 - 147 + y);

  } else {
    surface->put_screen((int)((x_pos-32 + x) * s), (int)((y_pos-16 + y) * s),
			s, s);  
  }
}


/* EOF */
