//  $Id: ice_block_obj.cxx,v 1.2 2002/09/15 11:02:24 grumbel Exp $
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

#include "../worldobjsdata/ice_block_data.hxx"
#include "ice_block_obj.hxx"

namespace EditorObjs {

IceBlockObj::IceBlockObj (WorldObjsData::IceBlockData* data_)
  : SpriteEditorObj ("iceblock", "worldobjs", &data_->pos),
    data(new WorldObjsData::IceBlockData(*data_))   
{
}

IceBlockObj::~IceBlockObj ()
{
  delete data;
}

/** Create the object with resonable defaults */
EditorObjLst
IceBlockObj::create (const CL_Vector& pos)
{
  WorldObjsData::IceBlockData newdata;
  newdata.pos = pos;
  return newdata.create_EditorObj();
}

std::string 
IceBlockObj::status_line ()
{
  char str[256];
  snprintf (str, 256, "IceBlock - %f %f %f", data->pos.x, data->pos.y, data->pos.z);
  return str;
}

void
IceBlockObj::write_xml (std::ostream& xml)
{
  data->write_xml(xml);
}

EditorObj*
IceBlockObj::duplicate ()
{
  return new IceBlockObj(data);
}

} // namespace EditorObjs

/* EOF */
