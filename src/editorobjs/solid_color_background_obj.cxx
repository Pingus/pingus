//  $Id: solid_color_background_obj.cxx,v 1.2 2002/09/16 20:52:22 torangan Exp $
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

#include "../string_converter.hxx"
#include "../editor/editor_view.hxx"
#include "../worldobjsdata/solid_color_background_data.hxx"
#include "solid_color_background_obj.hxx"

namespace EditorObjs {

SolidColorBackgroundObj::SolidColorBackgroundObj (WorldObjsData::SolidColorBackgroundData* data_)
  : data(new WorldObjsData::SolidColorBackgroundData(*data_))
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

EditorObj*
SolidColorBackgroundObj::duplicate ()
{
  return new SolidColorBackgroundObj(data);
}

float
SolidColorBackgroundObj::get_z_pos ()
{
  return data->pos.z;
}

CL_Vector
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
SolidColorBackgroundObj::set_position_offset (const CL_Vector& offset)
{
  data->pos += offset;
}  

std::string
SolidColorBackgroundObj::status_line ()
{
  return "SolidColorBackground: " 
	 + to_string(data->pos.x) + ", "
	 + to_string(data->pos.y) + ", "
	 + to_string(data->pos.z);
}

} // namespace EditorObjs

/* EOF */
