//  $Id: starfield_background_obj.cxx,v 1.2 2002/09/27 18:36:40 torangan Exp $
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

#include "../string_converter.hxx"
#include "starfield_background_obj.hxx"
#include "../worldobjsdata/starfield_background_data.hxx"

namespace EditorObjs {

StarfieldBackgroundObj::StarfieldBackgroundObj (const WorldObjsData::StarfieldBackgroundData& data_)
  : SpriteEditorObj ("Stars/starfield_icon", "game"),
    data(new WorldObjsData::StarfieldBackgroundData(data_)),
    pos (-64.0f, 0.0f)
{
  pos_ref = &pos;
}

StarfieldBackgroundObj::~StarfieldBackgroundObj ()
{
  delete data;
}

void
StarfieldBackgroundObj::write_xml (std::ostream& xml)
{
  data->write_xml (xml);
}

EditorObj*
StarfieldBackgroundObj::duplicate ()
{
  return new StarfieldBackgroundObj(*data);
}

std::string
StarfieldBackgroundObj::status_line ()
{
  return "StarfieldBackground: " 
	 + to_string (pos.x) + ", "
	 + to_string (pos.y) + ", "
	 + to_string (pos.z);
}


} // namespace EditorObjs

/* EOF */
