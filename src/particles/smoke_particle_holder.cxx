//  $Id: smoke_particle_holder.cxx,v 1.6 2003/10/19 12:25:47 grumbel Exp $
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

#include "../globals.hxx"
#include "../gui/graphic_context.hxx"
#include "../pingus_resource.hxx"
#include "smoke_particle_holder.hxx"

namespace Pingus {
namespace Particles {

SmokeParticleHolder::SmokeParticle::SmokeParticle (int x, int y, float vel_x, float vel_y)
  : pos(Vector(x,y)), velocity(Vector(vel_x, vel_y))
{
  time = livetime = 25 + (rand() % 10);
  use_surf2 = rand() % 2;
}


SmokeParticleHolder::SmokeParticleHolder ()
  : surf1(PingusResource::load_sprite("particles/smoke", "pingus")),
    surf2(PingusResource::load_sprite("particles/smoke2", "pingus"))
{
}


void
SmokeParticleHolder::add_particle (int x, int y, float vel_x, float vel_y)
{
  // search for dead entry to replace
  for (std::vector<SmokeParticle>::iterator it=particles.begin(); it != particles.end(); ++it)
    if (!it->livetime)
      {
        *it = SmokeParticle(x, y, vel_x, vel_y);
	    return;
	  }

	// create new entry
  particles.push_back(SmokeParticle(x, y, vel_x, vel_y));
}

void
SmokeParticleHolder::update ()
{
  // update all contained particles
  for (std::vector<SmokeParticle>::iterator it=particles.begin(); it != particles.end(); ++it)
    {
      // skip dead particles
      if (!it->livetime)
        continue;

      it->pos.x += it->velocity.x;
	  it->pos.y += it->velocity.y;

	  --it->livetime;
    }
}


void
SmokeParticleHolder::draw (GraphicContext& gc)
{
  for (std::vector<SmokeParticle>::iterator it=particles.begin(); it != particles.end(); ++it)
    {
      if (!it->livetime)
        continue;

      if (!it->use_surf2)
        gc.draw(surf1, it->pos);
	  else
	    gc.draw(surf2, it->pos);
    }
}

} // namespace Particles
} // namespace Pingus

/* EOF */
