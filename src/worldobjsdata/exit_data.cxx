//  $Id: exit_data.cxx,v 1.11 2003/10/18 23:17:28 grumbel Exp $
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
#include "../world.hxx"
#include "../xml_helper.hxx"
#include "../xml_file_reader.hxx"
#include "../worldobjs/exit.hxx"
#include "exit_data.hxx"

namespace Pingus {
namespace WorldObjsData {

ExitData::ExitData () : owner_id(0),
                        use_old_pos_handling(true)
{
}

ExitData::ExitData (xmlDocPtr doc, xmlNodePtr cur) : owner_id(0),
                                                     use_old_pos_handling(true)
{
  if (XMLhelper::get_prop(cur, "use-old-pos-handling", use_old_pos_handling))
    {
      if (use_old_pos_handling)
	 std::cout << "XMLPLF: Using Old Pos Handling" << std::endl;
    }

  XMLFileReader reader(doc, cur);
  reader.read_vector("position", pos);
  reader.read_desc("surface", desc);
  reader.read_int("owner-id", owner_id);
}

ExitData::ExitData (const ExitData& old) : WorldObjData(old),
                                           pos(old.pos),
					   desc(old.desc),
					   owner_id(old.owner_id),
					   use_old_pos_handling(old.use_old_pos_handling)
{
}

void
ExitData::write_xml (std::ostream& xml)
{
  xml << "<exit use-old-pos-handling=\"" << use_old_pos_handling << "\">\n";

  // FIXME: Repair me
  //pos.x += surf.get_width ()/2;
  //pos.y += surf.get_height ();
  XMLhelper::write_vector_xml(xml, pos);

  XMLhelper::write_desc_xml(xml, desc);
  xml << "  <owner-id>" << owner_id << "</owner-id>"
      << "</exit>\n"
      << std::endl;
}

void
ExitData::insert_WorldObjs (World* world)
{
  world->add_object(new WorldObjs::Exit(*this));
}

void
ExitData::insert_EditorObjs (EditorNS::EditorObjMgr* obj_mgr)
{
  //  obj_mgr->add(new EditorObjs::ExitObj(*this));
}

} // namespace WorldObjsData
} // namespace Pingus

/* EOF */

