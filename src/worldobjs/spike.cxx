//  $Id: spike.cxx,v 1.1 2002/09/04 14:55:13 torangan Exp $
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

#include "../pingus_resource.hxx"
#include "../pingu_holder.hxx"
#include "../world.hxx"
#include "../pingu.hxx"
#include "spike.hxx"
#include "../worldobjsdata/spike_data.hxx"

namespace WorldObjs {

  Spike::Spike (WorldObjsData::SpikeData* data_) : killing(false),
                                                   data (new WorldObjsData::SpikeData(*data_))
  {
    data->counter.set_size(data->surface.get_num_frames());
    data->counter.set_type(GameCounter::once);
    data->counter.set_speed(1);
    data->counter = 0;
  }

  Spike::~Spike ()
  {
    delete data;
  }

  float
  Spike::get_z_pos () const
  {
    return data->pos.z;
  }

  void
  Spike::draw_offset (int x, int y, float /*s*/)
  {
    if (killing) {
      data->surface.put_screen(static_cast<int>(data->pos.x + x),
                               static_cast<int>(data->pos.y + y),
			       data->counter);
    } else {
      // do nothing
    }
  }

  void
  Spike::update (float /*delta*/)
  {
    if (killing)
      ++data->counter;

    PinguHolder* holder = world->get_pingu_p ();
    for (PinguIter pingu = holder->begin (); pingu != holder->end (); ++pingu){
         catch_pingu(*pingu);
    }

    if (data->counter == static_cast<int>(data->surface.get_num_frames()) - 1) {
      killing = false;
      data->counter = 0;
    }
  }

  void
  Spike::catch_pingu (Pingu* pingu)
  {
    if (!killing) {
      if (   pingu->get_x () > data->pos.x + 16 - 5 && pingu->get_x () < data->pos.x + 16 + 5
	  && pingu->get_y () > data->pos.y          && pingu->get_y () < data->pos.y + 32) 
        {
	  data->counter = 0;
	  killing = true;
        }
    } else {
      if (   data->counter == 3 
          && pingu->get_x () > data->pos.x +16 - 12  && pingu->get_x () < data->pos.x + 16 + 12
	  && pingu->get_y () > data->pos.y           && pingu->get_y () < data->pos.y + 32) 
        {
	  pingu->set_status(PS_DEAD);
        }
    }  
  }

}

/* EOF */
