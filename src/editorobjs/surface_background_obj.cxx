//  $Id: surface_background_obj.cxx,v 1.3 2002/09/27 18:36:40 torangan Exp $
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
#include "../worldobjsdata/surface_background_data.hxx"
#include "surface_background_obj.hxx"

namespace EditorObjs {

SurfaceBackgroundObj::SurfaceBackgroundObj (const WorldObjsData::SurfaceBackgroundData& data_)
  : SpriteEditorObj (data_.desc.res_name, data_.desc.datafile),
    data(new WorldObjsData::SurfaceBackgroundData(data_))
{
  pos_ref = &data->pos;
}

SurfaceBackgroundObj::~SurfaceBackgroundObj ()
{
  delete data;
}

void
SurfaceBackgroundObj::write_xml (std::ostream& xml)
{
  data->write_xml(xml);
}

EditorObj*
SurfaceBackgroundObj::duplicate ()
{
  return new SurfaceBackgroundObj(*data);
}

std::string
SurfaceBackgroundObj::status_line ()
{
  return "SurfaceBackground: " 
         + to_string(data->pos.x) + ", "
         + to_string(data->pos.y) + ", "
         + to_string(data->pos.z);
}

} // namespace EditorObjs

/* EOF */
