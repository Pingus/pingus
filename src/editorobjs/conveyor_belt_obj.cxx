//  $Id: conveyor_belt_obj.cxx,v 1.10 2003/10/19 12:25:47 grumbel Exp $
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
#include "../editor/editor.hxx"
#include "../editor/editor_view.hxx"
#include "../editor/generic_property_frame.hxx"
#include "../editor/property_window.hxx"
#include "../pingus_resource.hxx"
#include "../worldobjsdata/conveyor_belt_data.hxx"
#include "conveyor_belt_obj.hxx"

namespace Pingus {
namespace EditorObjs {

ConveyorBeltObj::ConveyorBeltObj (const WorldObjsData::ConveyorBeltData& data_)
  : data(new WorldObjsData::ConveyorBeltData(data_)),
    left_sur  (PingusResource::load_sprite("conveyorbelt_left",   "worldobjs")),
    right_sur (PingusResource::load_sprite("conveyorbelt_right",  "worldobjs")),
    middle_sur(PingusResource::load_sprite("conveyorbelt_middle", "worldobjs"))
{
}

EditorNS::EditorObj*
ConveyorBeltObj::duplicate ()
{
  return new ConveyorBeltObj(*data);
}

void
ConveyorBeltObj::draw (EditorNS::EditorView * view)
{
  view->draw(left_sur, data->pos, static_cast<int>(data->counter));

#ifdef CLANLIB_0_6
  for (int i=0; i < data->width; ++i)
    {
      view->draw(middle_sur,
                 static_cast<int>(data->pos.x) + left_sur.get_frame_size(0).width 
                 + i * middle_sur.get_frame_size(0).width,
	         static_cast<int>(data->pos.y),
	         static_cast<int>(data->counter));
    }

  view->draw(right_sur,
	     static_cast<int>(data->pos.x) + left_sur.get_frame_size().width 
             + data->width * middle_sur.get_frame_size().width,
	     static_cast<int>(data->pos.y),
	     static_cast<int>(data->counter));

  data->counter += data->speed;
  if (data->counter > 14)
    data->counter = 0;
  else if (data->counter < 0)
    data->counter = middle_sur.get_num_frames() - 1;

#endif
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

EditorNS::EditorObjLst
ConveyorBeltObj::create (const Vector& pos)
{
  WorldObjsData::ConveyorBeltData newdata;
  newdata.pos = pos;
  return EditorNS::EditorObjLst(1, new ConveyorBeltObj(newdata));
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
  return   left_sur  .get_frame_size(0).width
         + right_sur .get_frame_size(0).width
	 + middle_sur.get_frame_size(0).width * data->width;
}

int
ConveyorBeltObj::get_height ()
{
#ifdef CLANLIB_0_6
  return middle_sur.get_height();
#else
  return 24;
#endif
}

float
ConveyorBeltObj::get_z_pos () {
  return data->pos.z;
}

void
ConveyorBeltObj::set_position_offset (const Vector& offset)
{
  data->pos += offset;
}

Vector
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

EditorNS::PropertyFrame*
ConveyorBeltObj::get_gui_dialog(EditorNS::Editor* editor)
{
  EditorNS::GenericPropertyFrame* propframe
    = new EditorNS::GenericPropertyFrame("ConveyorBelt Properties",
                                         editor->get_property_window()->get_client_area());

  propframe->add_integer_box("Width", &data->width);
  propframe->add_float_box("Speed", &data->speed);

  return propframe;
}

} // namespace EditorObjs
} // namespace Pingus

/* EOF */
