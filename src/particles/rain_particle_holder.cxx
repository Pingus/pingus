//  $Id: rain_particle_holder.cxx,v 1.7 2003/10/19 12:25:47 grumbel Exp $
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

#include "../display/scene_context.hxx"
#include "../math.hxx"
#include "../col_map.hxx"
#include "../globals.hxx"
#include "../resource.hxx"
#include "../world.hxx"
#include "rain_particle_holder.hxx"

namespace Particles {

RainParticleHolder::RainParticle::RainParticle(int x, int y)
  : alive(true), splash(false), use_rain2_surf(false), splash_counter(0), splash_frame(0), pos(Vector(x, y))
{
  use_rain2_surf = ((rand() % 3) == 0);
  pos.z = 1.0 + (float)Math::frand() * 3.0;
}


RainParticleHolder::RainParticleHolder ()
  : rain1_surf (Resource::load_sprite("particles/rain1")),
    rain2_surf (Resource::load_sprite("particles/rain2")),
    rain_splash(Resource::load_sprite("particles/rain_splash"))
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
	  if (it->splash_frame >= rain_splash.get_frame_count())
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
RainParticleHolder::draw (SceneContext& gc)
{
#ifdef CLANLIB_0_6
  for (std::vector<RainParticle>::iterator it=particles.begin(); it != particles.end(); ++it)
    {
      // skip dead/invisible particles
      if (!it->alive || it->pos.x > WorldObj::get_world()->get_width())
        continue;

      if (it->splash)
        gc.color().draw(rain_splash, it->pos, static_cast<int>(it->splash_frame));
      else
        if (it->use_rain2_surf)
          gc.color().draw(rain2_surf, static_cast<int>(it->pos.x), static_cast<int>(it->pos.y - rain1_surf.get_height()));
        else
          gc.color().draw(rain1_surf, static_cast<int>(it->pos.x), static_cast<int>(it->pos.y - rain1_surf.get_height()));
    }
#endif
}

} // namespace Particles

/* EOF */
