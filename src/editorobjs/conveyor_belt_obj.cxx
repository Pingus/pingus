//  $Id: conveyor_belt_obj.cxx,v 1.4 2002/09/27 18:36:40 torangan Exp $
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
#include "../editor/editor_view.hxx"
#include "../worldobjsdata/conveyor_belt_data.hxx"
#include "../pingus_resource.hxx"
#include "conveyor_belt_obj.hxx"

namespace EditorObjs {

ConveyorBeltObj::ConveyorBeltObj (const WorldObjsData::ConveyorBeltData& data_)
  : data(new WorldObjsData::ConveyorBeltData(data_)),
    left_sur  (PingusResource::load_surface ("conveyorbelt_left",   "worldobjs")),
    right_sur (PingusResource::load_surface ("conveyorbelt_right",  "worldobjs")),
    middle_sur(PingusResource::load_surface ("conveyorbelt_middle", "worldobjs"))
{
}

EditorObj*
ConveyorBeltObj::duplicate ()
{
  return new ConveyorBeltObj(*data);
}

void
ConveyorBeltObj::draw (EditorNS::EditorView * view)
{
  view->draw(left_sur, data->pos, static_cast<int>(data->counter));
  for (int i=0; i < data->width; ++i)
    {
      view->draw(middle_sur,
                 static_cast<int>(data->pos.x) + left_sur.get_width() + i * middle_sur.get_width(),
	         static_cast<int>(data->pos.y),
	         static_cast<int>(data->counter));
    }
    
  view->draw(right_sur,
	     static_cast<int>(data->pos.x) + left_sur.get_width() + data->width * middle_sur.get_width(),
	     static_cast<int>(data->pos.y),
	     static_cast<int>(data->counter));
	     
  data->counter += data->speed;
  if (data->counter > 14)
    data->counter = 0;
  else if (data->counter < 0)
    data->counter = middle_sur.get_num_frames() - 1;

}

void
ConveyorBeltObj::draw_scroll_map (int x_pos, int y_pos, int arg_width, int arg_height)
{
  // not supported
  UNUSED_ARG(x_pos);
  UNUSED_ARG(y_pos);
  UNUSED_ARG(arg_width);
  UNUSED_ARG(arg_height);
}

EditorObjLst
ConveyorBeltObj::create (const CL_Vector& pos)
{
  WorldObjsData::ConveyorBeltData newdata;
  newdata.pos = pos;
  return newdata.create_EditorObj();
}
  
std::string 
ConveyorBeltObj::status_line ()
{
  char str[256];
  snprintf (str, 256, "ConveyorBelt - (%f, %f, %f) Speed: %f", data->pos.x, data->pos.y, data->pos.z, data->speed);
  return str;
}

int 
ConveyorBeltObj::get_width ()
{
  return   left_sur  .get_width()
         + right_sur .get_width()
	 + middle_sur.get_width() * data->width;
}

int 
ConveyorBeltObj::get_height ()
{
  return middle_sur.get_height();
}

float
ConveyorBeltObj::get_z_pos () {
  return data->pos.z;
}

void 
ConveyorBeltObj::set_position_offset (const CL_Vector& offset)
{
  data->pos += offset;
}

CL_Vector
ConveyorBeltObj::get_upper_left_corner () {
  return data->pos;
}

void
ConveyorBeltObj::write_xml (std::ostream& xml)
{
  data->write_xml(xml);
}

void
ConveyorBeltObj::make_larger ()
{
  ++data->width;
}

void
ConveyorBeltObj::make_smaller ()
{
  if (data->width)
    --data->width;
}

} // namespace EditorObjs

/* EOF */
