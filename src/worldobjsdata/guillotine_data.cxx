//  $Id: guillotine_data.cxx,v 1.11 2003/03/05 19:13:59 grumbel Exp $
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

#include <iostream>
#include "guillotine_data.hxx"
#include "../world.hxx"
#include "../xml_helper.hxx"
#include "../xml_file_reader.hxx"
#include "../editorobjs/guillotine_obj.hxx"
#include "../worldobjs/guillotine.hxx"
#include "../pingus_resource.hxx"
#include "../editor/editorobj_mgr.hxx"

namespace WorldObjsData {

GuillotineData::GuillotineData ()
{
}

GuillotineData::GuillotineData (xmlDocPtr doc, xmlNodePtr cur) 
  : surface  (PingusResource::load_surface("Traps/guillotinekill", "traps")),
    idle_surf(PingusResource::load_surface("Traps/guillotineidle", "traps"))
{
  if (cur->name)
    std::cout << reinterpret_cast<const char*>(cur->name) << std::endl;
      
  XMLFileReader reader(doc, cur);
  reader.read_vector("position", pos);
}

GuillotineData::GuillotineData (const GuillotineData& old) : WorldObjData(old),
							     surface(old.surface),
							     idle_surf(old.idle_surf),	                                                 
							     pos(old.pos)
{
}

void
GuillotineData::write_xml (std::ostream& xml)
{
  xml << "<worldobj type=\"guillotine\">\n\t";
  XMLhelper::write_vector_xml(xml, pos);
  xml << "</worldobj>\n\n";
}

void
GuillotineData::insert_WorldObjs (World* world)
{
  world->add_object(new WorldObjs::Guillotine(*this));
}

void
GuillotineData::insert_EditorObjs (EditorNS::EditorObjMgr* obj_mgr)
{
  obj_mgr->add(new EditorObjs::GuillotineObj(*this));
}

} // namespace WorldObjsData

/* EOF */
