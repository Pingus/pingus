//  $Id: guillotine.cxx,v 1.6 2002/10/01 19:53:46 grumbel Exp $
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
#include "../worldobjsdata/guillotine_data.hxx"
#include "guillotine.hxx"

namespace WorldObjs {

Guillotine::Guillotine (const WorldObjsData::GuillotineData& data_) 
  : data(new WorldObjsData::GuillotineData(data_)),
    killing(false)
{
  data->counter.set_size(data->surface.get_num_frames()/2);
  data->counter.set_type(GameCounter::once);
  data->counter.set_speed(0.7);
  data->counter = 0;

  data->idle_counter.set_size(data->idle_surf.get_num_frames());
  data->idle_counter.set_type(GameCounter::loop);
  data->idle_counter.set_speed(0.3);
  data->idle_counter = 0;
}

Guillotine::~Guillotine ()
{
  delete data;
}

void
Guillotine::draw (GraphicContext& gc)
{
  if (killing) {
    if (data->direction.is_left())
      gc.draw(data->surface, data->pos, data->counter);
    else
      gc.draw (data->surface, data->pos, data->counter + 12);
  } else {
    gc.draw (data->idle_surf, data->pos, data->idle_counter);
  }   
}
  
  
float
Guillotine::get_z_pos () const
{
  return data->pos.z;
}

void
Guillotine::update ()
{
  if (data->counter.finished()) {
    data->counter = 0;
    killing = false;
  }

  PinguHolder* holder = world->get_pingu_p ();
  for (PinguIter pingu = holder->begin (); pingu != holder->end (); ++pingu)
    catch_pingu(*pingu);

  if (killing) {
    ++data->counter;
  } else {
    ++data->idle_counter;
  }
}

void
Guillotine::catch_pingu (Pingu* pingu)
{
  if (!killing) 
    {
      if (pingu->is_inside (static_cast<int>(data->pos.x + 38), static_cast<int>(data->pos.y + 90),
			    static_cast<int>(data->pos.x + 42), static_cast<int>(data->pos.y + 98)))
	{
	  killing = true;
	  pingu->set_status(PS_DEAD);
	  data->direction = pingu->direction;
	}
    }
}

} // namespace WorldObjs


/* EOF */
