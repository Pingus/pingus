//  $Id: hammer.cc,v 1.2 2000/02/09 21:43:45 grumbel Exp $
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

#include "hammer.hh"
#include "../particles/GroundParticle.hh"
#include "../PingusResource.hh"
#include "../algo.hh"
#include "../ActionHolder.hh"

Hammer::Hammer(trap_data data)
{
  x_pos = data.x_pos;
  y_pos = data.y_pos;
  z_pos = data.z_pos;
  surface = CL_Surface::load("Traps/hammer", PingusResource::get("traps.dat"));
  counter.set_size(surface->get_num_frames());
  counter.set_type(GameCounter::ping_pong);
  counter.set_speed(1);
}

Hammer::~Hammer()
{
}

void
Hammer::let_move()
{
  if (counter == 0) 
    particle_thrown = false;

  if (counter == (int)(surface->get_num_frames()) - 3 && !particle_thrown) {
    particle_thrown = true;
    for(int i=0; i < 5; ++i)
      particle->add_particle(new GroundParticle(x_pos + 67 + rand() % 40 - 20 ,
						y_pos + 177,
						frand() * 2 - 1,
						frand() * - 1.5));
  }
  ++counter;
}

bool
Hammer::catch_pingu(Pingu* pingu)
{
  if (counter >= (int)(surface->get_num_frames()) - 3) {
    if (pingu->get_x() > x_pos + 55 && pingu->get_x() < x_pos + 77
	&& pingu->get_y() > y_pos + 146 && pingu->get_y() < y_pos + 185)
      pingu->set_action(ActionHolder::get_uaction("smashed"));
    return true;
  } else {
    return false;
  }
}

/* EOF */
