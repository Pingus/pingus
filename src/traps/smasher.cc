//  $Id: smasher.cc,v 1.26 2001/08/12 18:36:42 grumbel Exp $
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

#include "../ColMap.hh"
#include "../World.hh"
#include "../ActionHolder.hh"
#include "../PinguHolder.hh"
#include "../PingusResource.hh"
#include "../PingusSound.hh"
#include "../particles/SmokeParticle.hh"
#include "../particles/ParticleHolder.hh"
#include "../actions/Splashed.hh"
#include "../algo.hh"
#include "../boost/smart_ptr.hpp"
#include "../GroundpieceData.hh"
#include "smasher.hh"

using boost::shared_ptr;

Smasher::Smasher(const TrapData& data)
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
	      PingusSound::play_wav("tenton", 0.7f);
	      
	      for(int i=0; i < 20; i++)
		{
		  world->get_particle_holder()
		    ->add_particle(new SmokeParticle(int(pos.x + 20 + rand() % 260),
						     int(pos.y + 180),
						     frand()-0.5, frand()-0.5));
		}

	      PinguHolder* holder = world->get_pingu_p();
	      for (PinguIter pingu = holder->begin (); pingu != holder->end (); ++pingu)
		{
		  if ((*pingu)->is_inside (int(pos.x + 30), int(pos.y + 90),
					   int(pos.x + 250), int(pos.y + 190)))
		    {
		      (*pingu)->set_action (shared_ptr<PinguAction>(new Splashed ()));
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
  world->get_colmap()->put(prov, (int) pos.x, (int) pos.y, GroundpieceData::GP_SOLID);
}

void 
Smasher::draw_offset(int x, int y, float s)
{
  surface.put_screen(int(pos.x + x), int(pos.y + y), count);
}

void 
Smasher::catch_pingu(boost::shared_ptr<Pingu> pingu)
{
  // Activate the smasher if a Pingu is under it
  if ((pingu->direction.is_left() 
       && pingu->get_x() > pos.x + 65 && pingu->get_x() < pos.x + 85)
      || 
      (pingu->direction.is_right() 
       && pingu->get_x() > pos.x + 190 && pingu->get_x() < pos.x + 210))
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
