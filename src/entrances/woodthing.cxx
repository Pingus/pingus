//  $Id: woodthing.cxx,v 1.3 2002/09/18 12:07:13 grumbel Exp $
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

#include "../particles/particle_holder.hxx"
#include "../world.hxx"
#include "../pingus_resource.hxx"
#include "../particles/smoke_particle.hxx"
#include "../graphic_context.hxx"
#include "../algo.hxx"

#include "woodthing.hxx"

WoodThing::WoodThing(EntranceData data)
  : Entrance (data)
{
  pos = data.pos;
  release_rate = data.release_rate;
  last_release = -release_rate;
  direction = data.direction;

  surface  = PingusResource::load_surface("Entrances/woodthing_mov", "entrances");
  surface2 = PingusResource::load_surface("Entrances/woodthing_nmov", "entrances");

  counter.set_size(surface.get_num_frames());
  counter.set_type(GameCounter::once);
  counter.set_speed(2);
  counter =  surface.get_num_frames() - 1;
}

void
WoodThing::update(float delta)
{
  Entrance::update (delta);

  --counter;

  if (rand() % 5 == 0) 
    {
      world->get_particle_holder()
	->add_particle(new SmokeParticle(int(pos.x - (surface.get_width()/2) - 24),
					 int(pos.y - surface.get_height() + 32 - 147),
					 -0.6 * (frand() + 1), -0.6 * (frand() + 1)));
    }
}

void
WoodThing::draw(GraphicContext& gc)
{
  gc.draw(surface, 
	  int(pos.x - (surface.get_width()/2)),
	  int(pos.y - surface.get_height() + 32),
	  counter);
}


/* EOF */
