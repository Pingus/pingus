//  $Id: info_box_obj.cxx,v 1.4 2002/09/28 11:52:25 torangan Exp $
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

#include "../worldobjsdata/info_box_data.hxx"
#include "info_box_obj.hxx"

namespace EditorObjs {

InfoBoxObj::InfoBoxObj (const WorldObjsData::InfoBoxData& data_)
  : SpriteEditorObj ("InfoBoxObj", "worldobjs"),
    data(new WorldObjsData::InfoBoxData(data_))
{
  pos_ref = &data->pos;
}

InfoBoxObj::~InfoBoxObj ()
{
  delete data;
}

EditorObjLst
InfoBoxObj::create (const Vector& pos)
{
  WorldObjsData::InfoBoxData newdata;
  newdata.pos = pos;
  return EditorObjLst(1, new InfoBoxObj(newdata));
}

void
InfoBoxObj::write_xml (std::ostream& xml)
{
  data->write_xml(xml);
}

EditorObj*
InfoBoxObj::duplicate ()
{
  return new InfoBoxObj(*data);
}

std::string 
InfoBoxObj::status_line ()
{
  return "InfoBoxObj";
}

} // namespace EditorObjs

/* EOF */
