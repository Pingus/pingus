//  $Id: switch_door.cxx,v 1.17 2002/09/16 20:31:09 grumbel Exp $
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

#include <fstream>
#include "../col_map.hxx"
#include "../graphic_context.hxx"
#include "../pingu.hxx"
#include "../pingu_holder.hxx"
#include "../world.hxx"
#include "../worldobjsdata/switch_door_data.hxx"
#include "../pingus_resource.hxx"
#include "switch_door.hxx"

namespace WorldObjs {

SwitchDoor::SwitchDoor (WorldObjsData::SwitchDoorData* data_) 
  : door_box      (PingusResource::load_surface("switchdoor_box"      , "worldobjs")),
    door_tile     (PingusResource::load_surface("switchdoor_tile"     , "worldobjs")),
    door_tile_cmap(PingusResource::load_surface("switchdoor_tile_cmap", "worldobjs")),
    switch_sur    (PingusResource::load_surface("switchdoor_switch"   , "worldobjs")),
    data(new WorldObjsData::SwitchDoorData(*data_)),
    is_opening(false),
    current_door_height(data->door_height)
{
}

SwitchDoor::~SwitchDoor ()
{
  delete data;
}

void 
SwitchDoor::draw_colmap ()
{
  world->get_colmap()->put(door_box,
                           static_cast<int>(data->door_pos.x),
			   static_cast<int>(data->door_pos.y),
			   Groundtype::GP_SOLID);
			   
  for (int i=0; i < data->door_height; ++i)
    world->get_colmap()->put(door_tile_cmap,
			     static_cast<int>(data->door_pos.x), 
			     static_cast<int>(data->door_pos.y) 
			     + i * door_tile.get_height()
			     + door_box.get_height(),
			     Groundtype::GP_SOLID);
}

void
SwitchDoor::draw (GraphicContext& gc)
{
  gc.draw (door_box, data->door_pos);
  for (int i=0; i < current_door_height; ++i)
    gc.draw(door_tile, 
	    static_cast<int>(data->door_pos.x),  
	    static_cast<int>(data->door_pos.y) 
	    + i * door_tile.get_height()
	    + door_box.get_height());

  gc.draw(switch_sur, data->switch_pos);
}

void
SwitchDoor::update (float /*delta*/)
{
  if (current_door_height > 0)
    {
      if (!is_opening)
	{
	  // Check if a pingu is passing the switch
	  PinguHolder* holder = world->get_pingu_p();
      
	  for (PinguIter pingu = holder->begin (); pingu != holder->end (); ++pingu)
	    {
	      if ((*pingu)->get_x()    > data->switch_pos.x
		  && (*pingu)->get_x() < data->switch_pos.x + static_cast<int>(switch_sur.get_width())
		  && (*pingu)->get_y() > data->switch_pos.y
		  && (*pingu)->get_y() < data->switch_pos.y + static_cast<int>(switch_sur.get_height()))
		{
		  is_opening = true;
		}
	    }
	}
      else
	{
	  // Open the door
	  --current_door_height;

	  // If the door is opend enough, so that a pingus fits under
	  // it, we remove the door from the colmap
	  if (current_door_height + 10 < data->door_height)
	    {
	      world->get_colmap()->put(door_box,
	                               static_cast<int>(data->door_pos.x),
				       static_cast<int>(data->door_pos.y),
				       Groundtype::GP_NOTHING);
	      for (int i=0; i < data->door_height; ++i)
		world->get_colmap()->put(door_tile_cmap,
					 static_cast<int>(data->door_pos.x), 
					 static_cast<int>(data->door_pos.y) + i * door_tile.get_height()
					                                    + door_box.get_height(),
					 Groundtype::GP_NOTHING);
	    }
	}
    }
}

} // namespace WorldObjs

/* EOF */
