//  $Id: solid_color_background_obj.cxx,v 1.9 2003/10/19 12:25:47 grumbel Exp $
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

#include <ClanLib/Core/System/clanstring.h>
#include "../editor/editor_view.hxx"
#include "../worldobjsdata/solid_color_background_data.hxx"
#include "../editor/editor.hxx"
#include "../editor/generic_property_frame.hxx"
#include "../editor/property_window.hxx"
#include "surface_background_obj.hxx"

#include "solid_color_background_obj.hxx"

namespace Pingus {

using namespace EditorNS;

namespace EditorObjs {

SolidColorBackgroundObj::SolidColorBackgroundObj (const WorldObjsData::SolidColorBackgroundData& data_)
  : data(new WorldObjsData::SolidColorBackgroundData(data_))
{
}

SolidColorBackgroundObj::~SolidColorBackgroundObj ()
{
  delete data;
}

void
SolidColorBackgroundObj::write_xml (std::ostream& xml)
{
  data->write_xml(xml);
}

EditorNS::EditorObj*
SolidColorBackgroundObj::duplicate ()
{
  return new SolidColorBackgroundObj(*data);
}

float
SolidColorBackgroundObj::get_z_pos ()
{
  return data->pos.z;
}

Vector
SolidColorBackgroundObj::get_upper_left_corner ()
{
  return data->pos;
}

void
SolidColorBackgroundObj::draw (EditorNS::EditorView* view)
{
  view->draw_fillrect(static_cast<int>(data->pos.x),
                      static_cast<int>(data->pos.y),
		      static_cast<int>(data->pos.x + 256),
		      static_cast<int>(data->pos.y + 256),
		      data->color.red,
		      data->color.green,
		      data->color.blue,
		      data->color.alpha);
}

void
SolidColorBackgroundObj::set_position_offset (const Vector& offset)
{
  data->pos += offset;
}

std::string
SolidColorBackgroundObj::status_line ()
{
  return "SolidColorBackground: "
	 + CL_String::to(data->pos.x) + ", "
	 + CL_String::to(data->pos.y) + ", "
	 + CL_String::to(data->pos.z);
}

EditorNS::PropertyFrame*
SolidColorBackgroundObj::get_gui_dialog(Editor* editor)
{
  GenericPropertyFrame* propframe
    = new GenericPropertyFrame("SolidColorBackground", editor->get_property_window()->get_client_area());

  propframe->add_float_box("Red", &data->color.red);
  propframe->add_float_box("Green", &data->color.green);
  propframe->add_float_box("Blue", &data->color.blue);
  propframe->add_float_box("Alpha", &data->color.alpha);

  return propframe;
}

} // namespace EditorObjs
} // namespace Pingus {

/* EOF */
