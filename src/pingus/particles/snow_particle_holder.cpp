// Pingus - A free Lemmings clone
// Copyright (C) 1999 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "pingus/particles/snow_particle_holder.hpp"

#include "engine/display/scene_context.hpp"
#include "pingus/collision_map.hpp"
#include "pingus/ground_map.hpp"
#include "pingus/world.hpp"

namespace Particles {

SnowParticleHolder::SnowParticle::SnowParticle (int x, int y, bool colliding_) :
  alive(true),
  colliding(colliding_),
  type(SnowParticleHolder::Snow1),
  pos(Vector3f(static_cast<float>(x),static_cast<float>(y))),
  velocity(Vector3f(0.0f, 1 + (Math::frand() * 3.5f)))
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
      break;
    default:
      type = SnowParticleHolder::Snow5;
      break;
  }
}

SnowParticleHolder::SnowParticleHolder() :
  snow1("particles/snow1"),
  snow2("particles/snow2"),
  snow3("particles/snow3"),
  snow4("particles/snow4"),
  snow5("particles/snow5"),
  ground("particles/ground_snow"),
  particles()
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
        world->get_gfx_map()->put(ground.get_surface(), static_cast<int>(it->pos.x - 1), static_cast<int>(it->pos.y - 1));
        it->alive = false;
      }
    }
  }
}

void
SnowParticleHolder::draw (SceneContext& gc)
{
  for (std::vector<SnowParticle>::iterator it=particles.begin(); it != particles.end(); ++it)
  {
    if (!it->alive)
      continue;

    switch (it->type)
    {
      case Snow1:
        gc.color().draw(snow1, it->pos);
        break;
      case Snow2:
        gc.color().draw(snow2, it->pos);
        break;
      case Snow3:
        gc.color().draw(snow3, it->pos);
        break;
      case Snow4:
        gc.color().draw(snow4, it->pos);
        break;
      case Snow5:
        gc.color().draw(snow5, it->pos);
        break;
      default:
        assert(false && "Invalid Snow-Type");
    }
  }
}

} // namespace Particles

/* EOF */
