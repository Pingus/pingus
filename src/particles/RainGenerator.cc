//  $Id: RainGenerator.cc,v 1.11 2002/06/01 18:05:37 torangan Exp $
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

#include <ClanLib/display.h>
#include "../World.hh"
#include "../particles/ParticleHolder.hh"
#include "../PinguMap.hh"
#include "RainParticle.hh"
#include "../PingusSound.hh"
#include "RainGenerator.hh"

RainGenerator::RainGenerator ()
  : do_thunder (false),
    thunder_count (0)
{
}

RainGenerator::~RainGenerator ()
{
}

void 
RainGenerator::draw_offset(int /*x*/, int /*y*/, float /*s*/)
{
  if (do_thunder)
    {
      if (thunder_count < 0.0f) {
	do_thunder = false;
	thunder_count = 0.0f;
	waiter_count = 1.0f;
      }

      CL_Display::fill_rect (0, 0, CL_Display::get_width (), CL_Display::get_height (),
			     1.0, 1.0, 1.0, thunder_count);
    }
}

void
RainGenerator::update(float delta)
{
  if (waiter_count < 0.0f && rand () % 150 == 0)
    {
      std::cout << "Doing thunder" << std::endl;
      do_thunder = true;
      thunder_count = 1.0f;
      waiter_count = 1.0f;
      PingusSound::play_sound ("sounds/thunder.wav");
    }
  
  if (do_thunder)
    thunder_count -= 10.0 * delta;

  waiter_count -= 20.0 * delta;

  world->get_particle_holder()->add_particle(new RainParticle(rand() % world->get_width(), -32));
  world->get_particle_holder()->add_particle(new RainParticle(rand() % world->get_width(), -32));
  world->get_particle_holder()->add_particle(new RainParticle(rand() % world->get_width(), -32));
  world->get_particle_holder()->add_particle(new RainParticle(rand() % world->get_width(), -32));
  world->get_particle_holder()->add_particle(new RainParticle(rand() % world->get_width(), -32));
  world->get_particle_holder()->add_particle(new RainParticle(rand() % world->get_width(), -32));
  world->get_particle_holder()->add_particle(new RainParticle(rand() % world->get_width(), -32));
  world->get_particle_holder()->add_particle(new RainParticle(rand() % world->get_width(), -32));
}

/* EOF */
