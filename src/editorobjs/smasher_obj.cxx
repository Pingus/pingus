//  $Id: smasher_obj.cxx,v 1.7 2002/09/27 18:36:40 torangan Exp $
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

#include <stdio.h>
#include "smasher_obj.hxx"
#include "../editor/editor_view.hxx"
#include "../worldobjsdata/smasher_data.hxx"

namespace EditorObjs {

SmasherObj::SmasherObj (const WorldObjsData::SmasherData& data_)
  : data(new WorldObjsData::SmasherData(data_)),
    frame(0)
{
  data->pos.z = -100;
  sprite = Sprite("Traps/smasher", "traps");
  pos_ref = &data->pos;
}

SmasherObj::~SmasherObj ()
{
  delete data;
}

EditorObjLst
SmasherObj::create (const CL_Vector& pos)
{
  WorldObjsData::SmasherData smasher_data;
  smasher_data.pos = pos;
  return EditorObjLst(1, new SmasherObj(smasher_data));
}
  
EditorObj*
SmasherObj::duplicate ()
{
  return new SmasherObj(*data);
}

void
SmasherObj::write_xml (std::ostream& xml)
{
  data->write_xml(xml);
}

void
SmasherObj::draw (EditorNS::EditorView* view)
{
  view->draw(sprite, data->pos, frame);
}

std::string  
SmasherObj::status_line ()
{
  char str[64];
  snprintf (str, 64, "SmasherObj: %4.2fx%4.2fx%4.2f", data->pos.x, data->pos.y, data->pos.z);
  return str;
}
  
} // namespace EditorObjs

/* EOF */
