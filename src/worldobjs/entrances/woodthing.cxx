//  $Id: woodthing.cxx,v 1.3 2002/11/05 03:02:48 grumbel Exp $
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

#include "../../algo.hxx"
#include "../../graphic_context.hxx"
#include "../../particles/particle_holder.hxx"
#include "../../particles/smoke_particle.hxx"
#include "../../pingus_resource.hxx"
#include "../../world.hxx"
#include "../../worldobjsdata/entrance_data.hxx"
#include "woodthing.hxx"

namespace WorldObjs {
namespace Entrances {

WoodThing::WoodThing (const WorldObjsData::EntranceData& data_)
  : Entrance(data_)
{
  surface  = PingusResource::load_surface("Entrances/woodthing_mov" , "entrances");
  surface2 = PingusResource::load_surface("Entrances/woodthing_nmov", "entrances");

  counter.set_size(surface.get_num_frames());
  counter.set_type(GameCounter::once);
  counter.set_speed(2);
  counter = surface.get_num_frames() - 1;
}

void
WoodThing::update ()
{
  Entrance::update ();

  --counter;

  if (rand() % 5 == 0) 
    {
      // FIXME: Memory leak
      world->get_particle_holder()
	   ->add_particle(new SmokeParticle(static_cast<int>(data->pos.x - surface.get_width () /  2 - 24),
        				    static_cast<int>(data->pos.y - surface.get_height() + 32 - 147),
					    -0.6 * (frand() + 1), -0.6 * (frand() + 1)));
    }
}

void
WoodThing::draw (GraphicContext& gc)
{
  gc.draw(surface, 
	  static_cast<int>(data->pos.x - surface.get_width () /  2),
	  static_cast<int>(data->pos.y - surface.get_height() + 32),
	  counter);
}

} // namespace Entrances
} // namespace WorldObjs

/* EOF */
