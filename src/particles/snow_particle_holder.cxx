//  $Id: snow_particle_holder.cxx,v 1.6 2003/02/19 09:50:36 grumbel Exp $
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

#include <assert.h>
#include "../math.hxx"
#include "../col_map.hxx"
#include "../gui/graphic_context.hxx"
#include "../pingu_map.hxx"
#include "../pingus_resource.hxx"
#include "../world.hxx"
#include "snow_particle_holder.hxx"

namespace Particles {

SnowParticleHolder::SnowParticle::SnowParticle (int x, int y, bool colliding_)
  : alive(true),
    colliding(colliding_),
    pos(Vector(x,y)),
    velocity(Vector(0.0f, 1 + (Math::frand() * 3.5f)))
{
  switch (rand() % 10)
    {
      case 0:
        type = SnowParticleHolder::Snow1;
	break;
      case 1:
	type = SnowParticleHolder::Snow2;
	break;
      case 2:
      case 3:
	type = SnowParticleHolder::Snow3;
	break;
      case 5:
      case 6:
	type = SnowParticleHolder::Snow4;
      default:
	type = SnowParticleHolder::Snow5;
	break;
    }
}


SnowParticleHolder::SnowParticleHolder ()
  : snow1 (PingusResource::load_surface("Particles/snow1",       "pingus")),
    snow2 (PingusResource::load_surface("Particles/snow2",       "pingus")),
    snow3 (PingusResource::load_surface("Particles/snow3",       "pingus")),
    snow4 (PingusResource::load_surface("Particles/snow4",       "pingus")),
    snow5 (PingusResource::load_surface("Particles/snow5",       "pingus")),
    ground(PingusResource::load_surface("Particles/ground_snow", "pingus"))
{
}


void
SnowParticleHolder::add_particle (int x, int y, bool colliding)
{
  // search for dead entry to replace
  for (std::vector<SnowParticle>::iterator it=particles.begin(); it != particles.end(); ++it)
    if (!it->alive)
      {
        *it = SnowParticle(x, y, colliding);
        return;
      }
	
	// create new entry
  particles.push_back(SnowParticle(x, y, colliding));
}

void
SnowParticleHolder::update ()
{
  // update all contained particles
  for (std::vector<SnowParticle>::iterator it=particles.begin(); it != particles.end(); ++it)
    {
      // skip dead particles
      if (!it->alive)
        continue;
      it->pos.x += it->velocity.x;
      it->pos.y += it->velocity.y;

      if (it->pos.y > world->get_height())
        {
          it->alive = false;
	  continue;
	}
      
      it->velocity.x += (Math::frand() - 0.5f) / 10;

      if (it->colliding)
        {
          int pixel = world->get_colmap()->getpixel(static_cast<int>(it->pos.x), static_cast<int>(it->pos.y));
          if ( pixel != Groundtype::GP_NOTHING
            && pixel != Groundtype::GP_WATER
	    && pixel != Groundtype::GP_OUTOFSCREEN)
	    {
	      world->get_gfx_map()->put(ground, static_cast<int>(it->pos.x - 1), static_cast<int>(it->pos.y - 1));
	      it->alive = false;
	    }
	}
    }

}


void
SnowParticleHolder::draw (GraphicContext& gc)
{
  for (std::vector<SnowParticle>::iterator it=particles.begin(); it != particles.end(); ++it)
    {
      if (!it->alive)
        continue;

      switch (it->type)
        {
	  case Snow1:
	    gc.draw(snow1, it->pos);
	    break;
	  case Snow2:
	    gc.draw(snow2, it->pos);
	    break;
	  case Snow3:
	    gc.draw(snow3, it->pos);
	    break;
	  case Snow4:
	    gc.draw(snow4, it->pos);
	    break;
	  case Snow5:
	    gc.draw(snow5, it->pos);
	    break;
	  default:
	    assert(!"Invalid Snow-Type");
        }
    }
}

} //namespace Particles

/* EOF */
