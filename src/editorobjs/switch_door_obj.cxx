//  $Id: switch_door_obj.cxx,v 1.6 2002/09/28 11:52:25 torangan Exp $
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

#include <stdio.h>
#include <iostream>
#include "switch_door_obj.hxx"
#include "../editor/editor_view.hxx"
#include "../pingus_resource.hxx"
#include "../worldobjsdata/switch_door_data.hxx"

namespace EditorObjs { 

SwitchDoorObj::SwitchDoorObj (const WorldObjsData::SwitchDoorData& data_)
  : data(new WorldObjsData::SwitchDoorData(data_)),
    door_box      (PingusResource::load_surface("switchdoor_box"      , "worldobjs")),
    door_tile     (PingusResource::load_surface("switchdoor_tile"     , "worldobjs")),
    door_tile_cmap(PingusResource::load_surface("switchdoor_tile_cmap", "worldobjs")),
    switch_sur    (PingusResource::load_surface("switchdoor_switch"   , "worldobjs"))
{
}

SwitchDoorObj::~SwitchDoorObj ()
{
  delete data;
}

EditorObj*
SwitchDoorObj::duplicate ()
{
  std::cout << "SwitchDoorObj::duplicate(): not implemented" << std::endl;
  return 0;
}

float
SwitchDoorObj::get_z_pos () {
  return data->door_pos.z;
}

int
SwitchDoorObj::get_width ()
{
  return door_box.get_width();
}

int
SwitchDoorObj::get_height ()
{
  return door_box.get_height();
}

/** Create this object (and child objects) with resonable defaults
    for the editor */
EditorObjLst
SwitchDoorObj::create (const Vector& pos)
{
  WorldObjsData::SwitchDoorData newdata;

  newdata.door_pos    = pos;
  newdata.switch_pos  = pos;
  newdata.door_height = 15;

  return EditorObjLst(1, new SwitchDoorObj(newdata));
}

void
SwitchDoorObj::save_xml (std::ostream& xml)
{
  data->write_xml(xml);
}

std::string 
SwitchDoorObj::status_line()
{
  char str[128];
  snprintf(str, 128, "SwitchDoor - (%f %f %f)", 
	  data->door_pos.x, data->door_pos.y, data->door_pos.z);
  return str;
}

void
SwitchDoorObj::draw (EditorNS::EditorView * view)
{
  view->draw_line(data->door_pos, data->switch_pos, 1.0, 0.0, 0.0);

  view->draw(door_box, 
             static_cast<int>(data->door_pos.x),
	     static_cast<int>(data->door_pos.y));

  for (int i = 0; i < data->door_height; ++i)
    {
      view->draw(door_tile, 
		 static_cast<int>(data->door_pos.x), 
		 static_cast<int>(data->door_pos.y 
				  + (i * door_tile.get_height())
				  + door_box.get_height()));
    }
}

void
SwitchDoorObj::make_larger ()
{
  data->door_height += 1;
}

void 
SwitchDoorObj::make_smaller ()
{
  if (data->door_height > 1)
    --data->door_height;
}

void
SwitchDoorObj::write_xml (std::ostream& xml)
{
  data->write_xml(xml);
}

Vector
SwitchDoorObj::get_upper_left_corner ()
{
  return data->door_pos;
}

void 
SwitchDoorObj::set_position_offset (const Vector& offset)
{
  data->door_pos += offset;
}

} // namespace EditorObjs

/* EOF */
