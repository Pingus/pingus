//  $Id: prefab_obj_data.cxx,v 1.10 2003/03/05 19:13:59 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#include <iostream>
#include "../xml_file_reader.hxx"
#include "../xml_helper.hxx"
#include "../prefab.hxx"
#include "prefab_obj_data.hxx"

namespace WorldObjsData {

PrefabObjData::PrefabObjData (xmlDocPtr doc, xmlNodePtr cur)
{
  XMLFileReader reader(doc, cur);
  reader.read_vector("position", pos);
  reader.read_string("type", type);
  
  // try to load the data for this prefab-uid
  data = Prefab::create (type);
}

void
PrefabObjData::insert_WorldObjs ()
{
  // FIXME:
}

void
PrefabObjData::insert_EditorObjs (EditorNS::EditorObjMgr* obj_mgr)
{
  UNUSED_ARG(obj_mgr);
  // FIXME:
}

} // namespace WorldObjsData

/* EOF */
