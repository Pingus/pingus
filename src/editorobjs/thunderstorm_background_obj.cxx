//  $Id: thunderstorm_background_obj.cxx,v 1.3 2002/09/28 11:52:25 torangan Exp $
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

#include "../worldobjsdata/thunderstorm_background_data.hxx"
#include "thunderstorm_background_obj.hxx"

namespace EditorObjs {

ThunderstormBackgroundObj::ThunderstormBackgroundObj (const WorldObjsData::ThunderstormBackgroundData& data_)
  : SpriteEditorObj ("Stars/starfield_icon", "game"),
    data(new WorldObjsData::ThunderstormBackgroundData(data_))
{
  data->pos = Vector(-128.0f, 0.0f);
  pos_ref = &data->pos;
}

ThunderstormBackgroundObj::~ThunderstormBackgroundObj ()
{
  delete data;
}

void
ThunderstormBackgroundObj::write_xml (std::ostream& xml)
{
  data->write_xml(xml);
}

EditorObj*
ThunderstormBackgroundObj::duplicate ()
{
  return new ThunderstormBackgroundObj(*data);
}

std::string
ThunderstormBackgroundObj::status_line ()
{
  return "ThunderstormBackground";
}

} // namespace EditorObjs

/* EOF */
