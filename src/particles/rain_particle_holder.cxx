//  $Id: rain_particle_holder.cxx,v 1.2 2002/12/31 15:09:33 torangan Exp $
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

#include "../algo.hxx"
#include "../col_map.hxx"
#include "../globals.hxx"
#include "../graphic_context.hxx"
#include "../pingus_resource.hxx"
#include "../world.hxx"
#include "rain_particle_holder.hxx"


RainParticleHolder::RainParticle::RainParticle(int x, int y)
  : alive(true), splash(false), use_rain2_surf(false), splash_counter(0), splash_frame(0), pos(Vector(x, y))
{
  use_rain2_surf = ((rand() % 3) == 0);
  pos.z = 1.0 + frand() * 3.0;
}


RainParticleHolder::RainParticleHolder ()
  : rain1_surf (PingusResource::load_surface("Particles/rain1", "pingus")),
    rain2_surf (PingusResource::load_surface("Particles/rain2", "pingus")),
    rain_splash(PingusResource::load_surface("Particles/rain_splash", "pingus")),
    world_width(1000.0f) // working default in case set_width() wasn't called
{
}


void
RainParticleHolder::add_particle (int x, int y)
{
  // search for dead entry to replace
  for (std::vector<RainParticle>::iterator it=particles.begin(); it != particles.end(); ++it)
    if (!it->alive)
      {
        *it = RainParticle(x, y);
        return;
      }
	
  // create new entry
  particles.push_back(RainParticle(x, y));
}

void
RainParticleHolder::update ()
{
  // update all contained particles
  for (std::vector<RainParticle>::iterator it=particles.begin(); it != particles.end(); ++it)
    {
      // skip dead particles
      if (!it->alive)
        continue;
	
      if (it->splash)
	{
	  if (it->splash_frame >= rain_splash.get_num_frames())
	    {
	      it->alive = false;
	      continue;
	    }
		  
	    it->splash_frame += 10 * game_speed / 1000.0f;
	    (it->splash_counter == 3) ? it->alive = false : ++it->splash_counter;
	}
      else
	{
	  if ( world->get_colmap()->getpixel(static_cast<int>(it->pos.x), static_cast<int>(it->pos.y)) != Groundtype::GP_NOTHING
	    && world->get_colmap()->getpixel(static_cast<int>(it->pos.x), static_cast<int>(it->pos.y)) != Groundtype::GP_OUTOFSCREEN
	    && ((rand() % 2) == 0))
            {
	      it->splash = true;
	    }
	  else
	    {
	      if (it->pos.y > world->get_height())
		{
		  it->alive = false;
		  continue;
		}
		
		it->pos.x -= 5  * it->pos.z;
		it->pos.y += 16 * it->pos.z;
	    }
	}
    }

}


void
RainParticleHolder::draw (GraphicContext& gc)
{
  for (std::vector<RainParticle>::iterator it=particles.begin(); it != particles.end(); ++it)
    {
      // skip dead/invisible particles
      if (!it->alive || it->pos.x > world_width)
        continue;

      if (it->splash)
        gc.draw(rain_splash, it->pos, static_cast<int>(it->splash_frame));
      else
        if (it->use_rain2_surf)
          gc.draw(rain2_surf, static_cast<int>(it->pos.x), static_cast<int>(it->pos.y - rain1_surf.get_height()));
        else
          gc.draw(rain1_surf, static_cast<int>(it->pos.x), static_cast<int>(it->pos.y - rain1_surf.get_height()));
    }
}

void
RainParticleHolder::set_world_width(int width)
{
  world_width = width;
}

/* EOF */
