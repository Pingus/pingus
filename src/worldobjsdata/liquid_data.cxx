//  $Id: liquid_data.cxx,v 1.12 2003/03/05 19:13:59 grumbel Exp $
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
#include "../editorobjs/liquid_obj.hxx"
#include "../worldobjs/liquid.hxx"
#include "../string_converter.hxx"
#include "../xml_helper.hxx"
#include "../xml_file_reader.hxx"
#include "../editor/editorobj_mgr.hxx"
#include "liquid_data.hxx"

namespace WorldObjsData {

LiquidData::LiquidData () 
  : old_width_handling(true),
    width(0),
    speed(20)
{
}

LiquidData::LiquidData (xmlDocPtr doc, xmlNodePtr cur)
  : old_width_handling(true),
    width(0),
    speed(20)
{
  if (XMLhelper::get_prop(cur, "use-old-width-handling", old_width_handling))
    {
      if (old_width_handling)
        std::cout << "XMLPLF: Using Old Width Handling: " << std::endl;
    }

  XMLFileReader reader(doc, cur);
  reader.read_vector("position", pos);
  reader.read_desc("surface", desc);
  reader.read_int("speed", speed);
  reader.read_int("width", width);
}

LiquidData::LiquidData (const LiquidData& old)
  : WorldObjData(old),
    desc(old.desc),
    pos(old.pos),
    old_width_handling(old.old_width_handling),
    width(old.width),
    speed(old.speed)
{
}


void 
LiquidData::write_xml (std::ostream& xml)
{
  std::cout << "LiquidData::write_xml(std::ostream& xml)" << std::endl;
  xml << "<liquid use-old-width-handling=\"" << old_width_handling << "\">\n";
  XMLhelper::write_desc_xml(xml, desc);
  XMLhelper::write_vector_xml(xml, pos);
  xml << "  <width>" << width << "</width>\n"
      << "  <speed>" << speed << "</speed>\n"
      << "</liquid>\n" << std::endl;
}

void
LiquidData::insert_WorldObjs (World* world)
{
  world->add_object(new WorldObjs::Liquid(*this));
}

void
LiquidData::insert_EditorObjs (EditorNS::EditorObjMgr* obj_mgr)
{ 
  obj_mgr->add(new EditorObjs::LiquidObj(*this));
}

} // namespace WorldObjsData

/* EOF */
