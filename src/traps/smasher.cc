//  $Id: smasher.cc,v 1.17 2001/04/01 18:00:43 grumbel Exp $
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
#include "../ActionHolder.hh"
#include "../PingusResource.hh"
#include "../PingusSound.hh"
#include "../particles/SmokeParticle.hh"
#include "../algo.hh"

#include "smasher.hh"

Smasher::Smasher(TrapData data)
{
  smashing = false;
  pos = data.pos;
  surface = PingusResource::load_surface("Traps/smasher", "traps");
  count = 0;
}

Smasher::~Smasher()
{
}

void
Smasher::update(float delta)
{
  if (smashing) 
    {
      if (downwards) 
	{
	  if (count >= 5) 
	    {
	      // SMASH!!! The thing hitten earth and kills the pingus
	      downwards = false;
	      --count; 
	      PingusSound::play_wav("tenton", 0.7);
	      
	      for(int i=0; i < 20; i++)
		{
		  world->get_particle_holder()
		    ->add_particle(new SmokeParticle(pos.x_pos + 20 + rand() % 260,
						     pos.y_pos + 180,
						     frand()-0.5, frand()-0.5));
		}

	      PinguHolder* holder = world->get_pingu_p();
	      for (PinguIter pingu = holder->begin (); pingu != holder->end (); ++pingu)
		{
		  if ((*pingu)->is_inside (pos.x_pos + 30, pos.y_pos + 90,
					pos.x_pos + 250, pos.y_pos + 190))
		    {
		      (*pingu)->set_status (dead);
		    }
		}
	    }
	  else 
	    {
	      ++count;
	    }
	} 
      else 
	{
	  if (count <= 0) {
	    count = 0;
	    smashing = false;
	  } else {
	    --count;
	  }
	}
    }
}

void
Smasher::draw_colmap()
{
  std::cout << "Drawing colmap entry" << std::endl;

  CL_Surface prov(PingusResource::load_surface("Traps/smasher_cmap", "traps"));
  world->get_colmap()->put(prov, pos.x_pos, pos.y_pos, GroundpieceData::SOLID);
}

void 
Smasher::draw_offset(int x, int y, float s)
{
  surface.put_screen(pos.x_pos + x, pos.y_pos + y, count);
}

void 
Smasher::catch_pingu(boost::shared_ptr<Pingu> pingu)
{
  // Activate the smasher if a Pingu is under it
  if ((pingu->direction.is_left() 
       && pingu->get_x() > pos.x_pos + 65 && pingu->get_x() < pos.x_pos + 85)
      || 
      (pingu->direction.is_right() 
       && pingu->get_x() > pos.x_pos + 190 && pingu->get_x() < pos.x_pos + 210))
    {
      if (!smashing) 
	{
	  count = 0;
	  downwards = true;
	  smashing = true; 
	}
    }
}

/* EOF */
