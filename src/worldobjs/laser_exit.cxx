//  $Id: laser_exit.cxx,v 1.8 2002/10/04 13:46:56 grumbel Exp $
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
#include "../pingu_action.hxx"
#include "../pingu_holder.hxx"
#include "../world.hxx"
#include "../worldobjsdata/laser_exit_data.hxx"
#include "laser_exit.hxx"

namespace WorldObjs {

LaserExit::LaserExit (const WorldObjsData::LaserExitData& data_) : data(new WorldObjsData::LaserExitData(data_)),
                                                                   killing(false)
{
  data->counter.set_size(data->surface.get_num_frames());
  data->counter.set_type(GameCounter::once);
  data->counter.set_speed(5);
  data->counter = 0;
}

LaserExit::~LaserExit ()
{
  delete data;
}

float
LaserExit::get_z_pos () const
{
  return data->pos.z;
}

void 
LaserExit::draw (GraphicContext& gc)
{
  gc.draw (data->surface, data->pos,
	   data->counter.value());
}

void
LaserExit::update ()
{

  PinguHolder* holder = world->get_pingus();
  for (PinguIter pingu = holder->begin (); pingu != holder->end (); ++pingu){
    catch_pingu(*pingu);
  }

  if (killing) {
    if (data->counter.finished()) {
      data->counter = 0;
      killing = false;
    } else {
      ++data->counter;
    }
  }
}

void
LaserExit::catch_pingu (Pingu* pingu)
{
  if (!killing) 
    {
      if (   pingu->get_x () < data->pos.x + 34 + 10 && pingu->get_x () > data->pos.x + 34 
	     && pingu->get_y () < data->pos.y + 43 + 20 && pingu->get_y () > data->pos.y + 43) 
	{
	  if (pingu->get_action()->get_type() != Actions::Laserkill) 
	    {
	      killing = true;
	      pingu->set_action(Actions::Laserkill);
	    }
	}
    }
}

} // namespace WorldObjs

/* EOF */
