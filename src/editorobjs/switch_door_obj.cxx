//  $Id: switch_door_obj.cxx,v 1.2 2002/09/14 19:06:34 torangan Exp $
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
#include "switch_door_obj.hxx"
#include "../editor/editor_view.hxx"
#include "../worldobjsdata/switch_door_data.hxx"

namespace EditorObjs { 

SwitchDoorObj::SwitchDoorObj (WorldObjsData::SwitchDoorData* data_)
                            : data(new WorldObjsData::SwitchDoorData(*data_))
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
  return data->door_box.get_width();
}

int
SwitchDoorObj::get_height ()
{
  return data->door_box.get_height();
}

/** Create this object (and child objects) with resonable defaults
    for the editor */
EditorObjLst
SwitchDoorObj::create (const CL_Vector& pos)
{
  WorldObjsData::SwitchDoorData newdata;

  newdata.door_pos    = pos;
  newdata.switch_pos  = pos;
  newdata.door_height = 15;

  return newdata.create_EditorObj();
}

void
SwitchDoorObj::save_xml (std::ostream& xml)
{
  write_xml (xml);
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

  view->draw(data->door_box, 
             static_cast<int>(data->door_pos.x),
	     static_cast<int>(data->door_pos.y));

  for (int i = 0; i < data->door_height; ++i)
    {
      view->draw(data->door_tile, 
		 static_cast<int>(data->door_pos.x), 
		 static_cast<int>(data->door_pos.y + (i * data->door_tile.get_height())
  	                                           + data->door_box.get_height()));
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

CL_Vector
SwitchDoorObj::get_upper_left_corner ()
{
  return data->door_pos;
}

void 
SwitchDoorObj::set_position_offset (const CL_Vector& offset)
{
  data->door_pos += offset;
}

} // namespace EditorObjs

/* EOF */
