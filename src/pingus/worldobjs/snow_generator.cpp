// Pingus - A free Lemmings clone
// Copyright (C) 2000 Ingo Ruhnke <grumbel@gmail.com>
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

#include "pingus/worldobjs/snow_generator.hpp"

#include "pingus/globals.hpp"
#include "pingus/particles/snow_particle_holder.hpp"
#include "pingus/world.hpp"

namespace WorldObjs {

SnowGenerator::SnowGenerator(const ReaderMapping& reader) :
  intensity(1.0f)
{
  reader.read_float("intensity", intensity);
}

SnowGenerator::~SnowGenerator()
{
}

void
SnowGenerator::update()
{
  for(int i = 0; i < Math::floor(intensity); ++i)
  {
    if (rand() % 3 != 0)
      world->get_snow_particle_holder()->add_particle(rand() % world->get_width(), -globals::tile_size, false);
    else
      world->get_snow_particle_holder()->add_particle(rand() % world->get_width(), -globals::tile_size, true);
  }

  if ((intensity - static_cast<float>(static_cast<int>(intensity))) > Math::frand())
  {
    if (rand() % 3 != 0)
      world->get_snow_particle_holder()->add_particle(rand() % world->get_width(), -globals::tile_size, false);
    else
      world->get_snow_particle_holder()->add_particle(rand() % world->get_width(), -globals::tile_size, true);
  }
}

void
SnowGenerator::on_startup()
{
  // FIXME: if snow should be available from start, this needs to get fixed to have the right amount of snow
#if 0
  for (int i = 0; i < 500; ++i) // FIXME: 500 is a random value, doesn't work very often
  {
    world->get_snow_particle_holder()->add_particle(rand() % world->get_width(), rand() % world->get_height());
    world->get_snow_particle_holder()->add_particle(rand() % world->get_width(), rand() % world->get_height());
    world->get_snow_particle_holder()->add_particle(rand() % world->get_width(), rand() % world->get_height(), true);
  }
#endif
}

} // namespace WorldObjs

/* EOF */
