//  $Id: hammer.cc,v 1.18 2002/06/08 16:08:16 grumbel Exp $
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

#include "../World.hh"
#include "hammer.hh"
#include "../particles/GroundParticle.hh"
#include "../PingusResource.hh"
#include "../algo.hh"
#include "../ActionHolder.hh"
#include "../PinguActionFactory.hh"
#include "../PinguHolder.hh"

Hammer::Hammer(const TrapData& data)
{
  pos = data.pos;

  surface = PingusResource::load_surface("Traps/hammer", "traps");
  counter.set_size(surface.get_num_frames());
  counter.set_type(GameCounter::ping_pong);
  counter.set_speed(1);
}

Hammer::~Hammer()
{
}

void
Hammer::update(float /*delta*/)
{
  if (counter == 0) 
    particle_thrown = false;

  PinguHolder* holder = world->get_pingu_p ();
  for (PinguIter pingu = holder->begin (); pingu != holder->end (); ++pingu){
       catch_pingu(*pingu);
  }

  if (counter == (int)(surface.get_num_frames()) - 3 && !particle_thrown) {
    particle_thrown = true;
    /*
    for(int i=0; i < 5; ++i)
      particle->add_particle(new GroundParticle(x_pos + 67 + rand() % 40 - 20 ,
						y_pos + 177,
						frand() * 2 - 1,
						frand() * - 1.5));
    */
  }
  ++counter;
}

void
Hammer::catch_pingu(Pingu* pingu)
{
  if (counter >= (int)(surface.get_num_frames()) - 3) {
    if (pingu->get_x() > pos.x + 55 && pingu->get_x() < pos.x + 77
	&& pingu->get_y() > pos.y + 146 && pingu->get_y() < pos.y + 185)
      pingu->set_action(PinguActionFactory::instance ()->create_sp ("smashed"));
  }
}

/* EOF */
