//  $Id: hammer.cxx,v 1.5 2002/09/27 18:36:41 torangan Exp $
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

#include "../graphic_context.hxx"
#include "../pingu.hxx"
#include "../pingu_holder.hxx"
#include "../world.hxx"
#include "../worldobjsdata/hammer_data.hxx"
#include "hammer.hxx"

namespace WorldObjs {

Hammer::Hammer (const WorldObjsData::HammerData& data_) : data(new WorldObjsData::HammerData(data_)),
                                                          particle_thrown(false)
{
  data->counter.set_size(data->surface.get_num_frames());
  data->counter.set_type(GameCounter::ping_pong);
  data->counter.set_speed(1);
}

Hammer::~Hammer ()
{
  delete data;
}

float
Hammer::get_z_pos () const
{
  return data->pos.z;
}

void 
Hammer::draw (GraphicContext& gc)
{
  gc.draw (data->surface, data->pos, data->counter.value());
}

void
Hammer::update (float delta)
{
  if ( !data->counter) 
    particle_thrown = false;

  PinguHolder* holder = world->get_pingu_p ();
  for (PinguIter pingu = holder->begin (); pingu != holder->end (); ++pingu){
    catch_pingu(*pingu);
  }

  if ( !particle_thrown && data->counter == static_cast<int>(data->surface.get_num_frames() - 3)) {
    particle_thrown = true;
    /*
      for(int i=0; i < 5; ++i)
      particle->add_particle(new GroundParticle(x_pos + 67 + rand() % 40 - 20 ,
      y_pos + 177,
      frand() * 2 - 1,
      frand() * - 1.5));
    */
  }
  ++data->counter;
  UNUSED_ARG(delta);
}

void
Hammer::catch_pingu (Pingu* pingu)
{
  if (data->counter >= static_cast<int>(data->surface.get_num_frames() - 3)) {
    if (  pingu->get_x() > data->pos.x + 55  && pingu->get_x() < data->pos.x + 77
	  && pingu->get_y() > data->pos.y + 146 && pingu->get_y() < data->pos.y + 185)
      pingu->set_action(Actions::Smashed);
  }
}

} // namespace WorldObjs

/* EOF */
