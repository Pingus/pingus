//  $Id: smasher_data.cxx,v 1.13 2003/10/20 13:11:09 grumbel Exp $
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
#include "smasher_data.hxx"
#include "../world.hxx"
#include "../xml_helper.hxx"
#include "../xml_file_reader.hxx"
#include "../editorobjs/smasher_obj.hxx"
#include "../worldobjs/smasher.hxx"
#include "../editor/editorobj_mgr.hxx"
#include "../resource.hxx"

namespace Pingus {
namespace WorldObjsData {

SmasherData::SmasherData ()
{
}

SmasherData::SmasherData (xmlDocPtr doc, xmlNodePtr cur) 
  : surface(Resource::load_sprite("traps/smasher"))
{
  XMLFileReader reader(doc, cur);
  reader.read_vector("position", pos);
}

SmasherData::SmasherData (const SmasherData& old) : WorldObjData(old),
						    surface(old.surface),
						    pos(old.pos)
{
}

void
SmasherData::write_xml (std::ostream& xml)
{
  xml << "<worldobj type=\"smasher\">\n\t";
  XMLhelper::write_vector_xml(xml, pos);
  xml << "</worldobj>\n\n";
}

void
SmasherData::insert_WorldObjs (World* world)
{
  world->add_object(new WorldObjs::Smasher(*this));
}

void
SmasherData::insert_EditorObjs (EditorNS::EditorObjMgr* obj_mgr)
{
  //  obj_mgr->add(new EditorObjs::SmasherObj(*this));
}

} // namespace WorldObjsData
} // namespace Pingus

/* EOF */
