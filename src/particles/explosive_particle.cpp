//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#if 0
#include "../collision_map.hpp"
#include "../pingu_map.hpp"
#include "../world.hpp"
#include "../particles/particle_holder.hpp"
#include "../resource.hpp"
#include "explosive_particle.hpp"

ExplosiveParticle::ExplosiveParticle (int x, int y, float x_a, float y_a)
                                    : Particle (x, y, x_a, y_a),
                                      alive (true)
{
}

ExplosiveParticle::~ExplosiveParticle ()
{
}

void
ExplosiveParticle::update (float delta)
{
  Vector new_pos = pos + velocity * delta;

  Vector incr = pos - new_pos;
  incr.normalize ();

  // FIXME: This thing needs to be more abstract, we just need it far
  // to often to reimplement it over and over again.
  while (   static_cast<int>(new_pos.x) != static_cast<int>(pos.x)
	 || static_cast<int>(new_pos.y) != static_cast<int>(pos.y))
    {
      pos -= incr;

      if (   pos.x < 0
          || pos.y < 0
	  || pos.x + 1 > WorldObj::get_world()->get_width ()
	  || pos.y + 1 > WorldObj::get_world()->get_height())
	{
	  alive = false;
	  return;
	}

      if (WorldObj::get_world()->get_colmap()->getpixel(static_cast<int>(pos.x), static_cast<int>(pos.y)))
	{
	  detonate();
	}
    }

  pos = new_pos;
}

void
ExplosiveParticle::detonate ()
{
  alive = false;
  WorldObj::get_world()->get_particle_holder ()->add_pingu_explo((int)pos.x, (int)pos.y);

  CollisionMask mask("Other/bomber_radius", "pingus");
  WorldObj::get_world()->remove(mask,
                                int(pos.x) - (bomber_radius.get_width()/2),
                                int(pos.y) - (bomber_radius.get_height()/2));
}

//void
//ExplosiveParticle::draw_offset(int ofx, int ofy, float /*s*/)
//{
//  sprite.put_screen (int(pos.x + ofx), int(pos.y + ofy));
//}


bool
ExplosiveParticle::is_alive(void)
{
  return alive;
}

/* EOF */
#endif
