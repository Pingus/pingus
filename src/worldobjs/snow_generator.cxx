//  $Id: snow_generator.cxx,v 1.2 2002/09/16 16:04:58 grumbel Exp $
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

#include "../world.hxx"
#include "../particles/snow_particle.hxx"
#include "../particles/particle_holder.hxx"
#include "snow_generator.hxx"

namespace WorldObjs {

SnowGenerator::SnowGenerator()
{
}

SnowGenerator::~SnowGenerator()
{
}

void 
SnowGenerator::update(float /*delta*/)
{
  world->get_particle_holder()->add_particle(new SnowParticle(rand() % world->get_width(), -32));
  world->get_particle_holder()->add_particle(new SnowParticle(rand() % world->get_width(), -32));
  world->get_particle_holder()->add_particle(new CollidingSnowParticle(rand() % world->get_width(), -32));
}

} // namespace WorldObjs

/* EOF */
