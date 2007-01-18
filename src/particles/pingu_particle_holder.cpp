//  $Id: pingu_particle_holder.cxx,v 1.10 2003/10/19 12:25:47 grumbel Exp $
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

#include "../display/scene_context.hpp"
#include "../math.hpp"
#include "../col_map.hpp"
#include "../resource.hpp"
#include "../world.hpp"
#include "pingu_particle_holder.hpp"

namespace Particles {

const float x_collision_decrease = 0.3f;
const float y_collision_decrease = 0.6f;

PinguParticleHolder::PinguParticle::PinguParticle (int x, int y)
  : livetime(50 + (rand() % 75)),
    use_frame2((rand() % 5) == 0),
    pos(Vector3f(x, y)),
    velocity(Vector3f((float)Math::frand() * 7 - 3.5, (float)Math::frand() * -9))
{
}


PinguParticleHolder::PinguParticleHolder ()
  : surface(Resource::load_sprite("particles/pingu_explo"))
{
}


void
PinguParticleHolder::add_particle (int x, int y)
{
  int i = 0;

  // fill gaps from dead entries
  for (std::vector<PinguParticle>::iterator it=particles.begin(); it != particles.end(); ++it)
    {
      if (!it->livetime)
        {
          *it = PinguParticle(x, y);
          ++i;
        }
    }

  // allocate space for all remaining particles at once
  particles.reserve(particles.size() + 50 - i);

  // create remaining entries
  for (; i < 50; ++i)
    particles.push_back(PinguParticle(x, y));
}

void
PinguParticleHolder::update ()
{
  // update all contained particles
  for (std::vector<PinguParticle>::iterator it=particles.begin(); it != particles.end(); ++it)
    {
      // skip dead particles
      if (!it->livetime)
        continue;

      float tmp_x_add = 0.0f;
      float tmp_y_add = 0.0f;

      // Simulated gravity
      it->velocity.y += WorldObj::get_world()->get_gravity();

      if (it->velocity.y > 0)
        {
          for (tmp_y_add = it->velocity.y; tmp_y_add >= 1.0; --tmp_y_add)
            {
	      if (world->get_colmap()->getpixel(static_cast<int>(it->pos.x), static_cast<int>(it->pos.y)))
	        {
	          it->velocity.y *= -y_collision_decrease;
	          tmp_y_add = -tmp_y_add;
	          --it->pos.y;
	          break;
	        }
	      ++it->pos.y;
            }
          it->pos.y += tmp_y_add;
        }
      else
        {
          for (tmp_y_add = it->velocity.y; tmp_y_add <= -1.0; ++tmp_y_add)
            {
	      if (world->get_colmap()->getpixel(static_cast<int>(it->pos.x), static_cast<int>(it->pos.y)))
		      {
	          it->velocity.y *= -y_collision_decrease;
	          tmp_y_add = -tmp_y_add;
	          ++it->pos.y;
	          break;
	        }
	      --it->pos.y;
            }
          it->pos.y += tmp_y_add;
        }


      if (it->velocity.x > 0)
        {
          for (tmp_x_add = it->velocity.x; tmp_x_add >= 1.0; --tmp_x_add)
            {
	      if (world->get_colmap()->getpixel(static_cast<int>(it->pos.x), static_cast<int>(it->pos.y)))
		      {
	          it->velocity.x *= -x_collision_decrease;
	          tmp_x_add = -tmp_x_add;
	          --it->pos.x;
	          break;
	        }
	      ++it->pos.x;
            }
          it->pos.x += tmp_x_add;
        }
      else
        {
          for (tmp_x_add = it->velocity.x; tmp_x_add <= -1.0; ++tmp_x_add)
            {
              if (world->get_colmap()->getpixel(static_cast<int>(it->pos.x), static_cast<int>(it->pos.y)))
	        {
	          it->velocity.x *= -x_collision_decrease;
	          tmp_x_add = -tmp_x_add;
	          ++it->pos.x;
	          break;
	        }
	      --it->pos.x;
            }
          it->pos.x += tmp_x_add;
        }

      --it->livetime;
    }
}


void
PinguParticleHolder::draw (SceneContext& gc)
{
  for (std::vector<PinguParticle>::iterator it=particles.begin(); it != particles.end(); ++it)
    {
      // skip dead particles
      if (!it->livetime)
        continue;

      gc.color().draw(surface, it->pos);
    }
}

} // namespace Particles

/* EOF */
