//  $Id: snow_generator.cxx,v 1.6 2003/02/19 17:17:01 grumbel Exp $
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

#include <math.h>
#include <iostream>
#include "../math.hxx"
#include "../world.hxx"
#include "../particles/snow_particle_holder.hxx"
#include "../worldobjsdata/snow_generator_data.hxx"
#include "snow_generator.hxx"

namespace WorldObjs {

SnowGenerator::SnowGenerator (const WorldObjsData::SnowGeneratorData& data_)
{
  intensity = data_.intensity;
}

SnowGenerator::~SnowGenerator()
{
}

void 
SnowGenerator::update()
{
  for(int i = 0; i < floor(intensity); ++i)
    {
      if (rand() % 3 != 0)
        world->get_snow_particle_holder()->add_particle(rand() % world->get_width(), -32, false);
      else
        world->get_snow_particle_holder()->add_particle(rand() % world->get_width(), -32, true);
    }

  if ((intensity - (int)intensity) > Math::frand())
    {
      if (rand() % 3 != 0)
        world->get_snow_particle_holder()->add_particle(rand() % world->get_width(), -32, false);
      else
        world->get_snow_particle_holder()->add_particle(rand() % world->get_width(), -32, true);
    }
}

void
SnowGenerator::on_startup()
{
  for (int i = 0; i < 500; ++i) // FIXME: 500 is a random value, doesn't work very often
    {
      world->get_snow_particle_holder()->add_particle(rand() % world->get_width(), rand() % world->get_height());
      world->get_snow_particle_holder()->add_particle(rand() % world->get_width(), rand() % world->get_height());
      world->get_snow_particle_holder()->add_particle(rand() % world->get_width(), rand() % world->get_height(), true); 
    }
}

} // namespace WorldObjs

/* EOF */
