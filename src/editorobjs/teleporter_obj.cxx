//  $Id: teleporter_obj.cxx,v 1.9 2003/03/30 13:12:35 grumbel Exp $
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
#include "../debug.hxx"
#include "teleporter_obj.hxx"
#include "../editor/editor_view.hxx"
#include "../worldobjsdata/teleporter_data.hxx"

namespace EditorObjs {

TeleporterObj::TeleporterObj (const WorldObjsData::TeleporterData& data_)
  : SpriteEditorObj ("teleporter", "worldobjs"),
    data(new WorldObjsData::TeleporterData(data_))
{
  pos_ref = &data->pos;
}

TeleporterObj::~TeleporterObj ()
{
  delete data;
}

Vector&
TeleporterObj::get_target_pos_ref ()
{
  return data->target_pos;
}

EditorObj*
TeleporterObj::duplicate()
{
  pwarn << "EditorTeleporterObj::duplicate(): not implemented" << std::endl;
  return 0;
}

EditorObjLst
TeleporterObj::create (const Vector& pos)
{
  WorldObjsData::TeleporterData newdata;

  newdata.pos          = pos;
  newdata.target_pos.x = pos.x + 50;
  newdata.target_pos.y = pos.y + 50;

  return EditorObjLst(1, new TeleporterObj(newdata));
}

void
TeleporterObj::draw (EditorNS::EditorView * view)
{
  view->draw_line (static_cast<int>(data->pos.x), 
		   static_cast<int>(data->pos.y),
		   static_cast<int>(data->target_pos.x), 
		   static_cast<int>(data->target_pos.y),
		   0.0, 1.0, 0.0, 0.5);
  SpriteEditorObj::draw (view);
}

void
TeleporterObj::write_xml (std::ostream& xml) {
  data->write_xml(xml);
}

std::string
TeleporterObj::status_line ()
{
  char str[128];
  snprintf (str, 128, "Teleporter - %f %f %f", data->pos.x, data->pos.y, data->pos.z);
  return str;
}

} // namespace EditorObjs

/* EOF */
