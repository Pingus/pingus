//  $Id: conveyor_belt.cxx,v 1.18 2002/09/28 11:52:26 torangan Exp $
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

#include "../col_map.hxx"
#include "../graphic_context.hxx"
#include "../pingu.hxx"
#include "../pingu_holder.hxx"
#include "../pingus_resource.hxx"
#include "../world.hxx"
#include "../worldobjsdata/conveyor_belt_data.hxx"
#include "conveyor_belt.hxx"

namespace WorldObjs {

ConveyorBelt::ConveyorBelt (const WorldObjsData::ConveyorBeltData& data_)
  : data(new WorldObjsData::ConveyorBeltData(data_)),
    left_sur  (PingusResource::load_surface ("conveyorbelt_left",   "worldobjs")),
    right_sur (PingusResource::load_surface ("conveyorbelt_right",  "worldobjs")),
    middle_sur(PingusResource::load_surface ("conveyorbelt_middle", "worldobjs"))
{
}

void
ConveyorBelt::draw (GraphicContext& gc)
{
  gc.draw(left_sur, data->pos, static_cast<int>(data->counter));
  for (int i=0; i < data->width; ++i)
    gc.draw(middle_sur, 
	    static_cast<int>(data->pos.x + left_sur.get_width() + i * middle_sur.get_width()),
	    static_cast<int>(data->pos.y), 
	    static_cast<int>(data->counter));
  
  gc.draw(right_sur,
	  static_cast<int>(data->pos.x + left_sur.get_width() + data->width * middle_sur.get_width()),
	  static_cast<int>(data->pos.y),
	  static_cast<int>(data->counter));
}

void
ConveyorBelt::on_startup ()
{
  CL_Surface sur(PingusResource::load_surface("conveyorbelt_cmap", "worldobjs"));
  for (int i=0; i < (data->width + 2); ++i)
    world->get_colmap()->put(sur,
                             static_cast<int>(data->pos.x) + (15 * i),
			     static_cast<int>(data->pos.y),
			     Groundtype::GP_SOLID);
}

void 
ConveyorBelt::update (float delta)
{
  data->counter += data->speed * delta;

  if (data->counter >= 14.0f)
    data->counter = 0.0f;
  else if (data->counter < 0.0f)
    data->counter = middle_sur.get_num_frames() - 1;

  PinguHolder* holder = world->get_pingu_p();
  for (PinguIter pingu = holder->begin(); pingu != holder->end(); ++pingu)
    {
      if (   (*pingu)->get_x() > data->pos.x
	  && (*pingu)->get_x() < data->pos.x + 15 * (data->width + 2)
	  && (*pingu)->get_y() > data->pos.y - 2
	  && (*pingu)->get_y() < data->pos.y + 10)
	{
	  Vector pos = (*pingu)->get_pos();
	  data->pos.x -= data->speed * delta;
	  (*pingu)->set_pos(pos);
	}
    }
}

float
ConveyorBelt::get_z_pos () const
{
  return data->pos.z;
}

} // namespace WorldObjs

/* EOF */
