//  $Id: hotspot_data.cxx,v 1.5 2003/02/26 17:08:29 grumbel Exp $
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
#include "../editorobjs/hotspot_obj.hxx"
#include "../worldobjs/hotspot.hxx"
#include "../xml_helper.hxx"
#include "../xml_file_reader.hxx"
#include "hotspot_data.hxx"

namespace WorldObjsData {

HotspotData::HotspotData () : speed(-1), para(1.0)
{
}

void 
HotspotData::write_xml(std::ostream& xml)
{
  xml << "<hotspot>\n";
  XMLhelper::write_desc_xml(xml, desc);
  XMLhelper::write_vector_xml(xml, pos);
  xml << "  <speed>"    << speed << "</speed>\n"
      << "  <parallax>" << para  << "</parallax>\n"
      << "</hotspot>\n" << std::endl;  
}

HotspotData::HotspotData (xmlDocPtr doc, xmlNodePtr cur)
{
  XMLFileReader reader(doc, cur);
  reader.read_vector("position", pos);
  reader.read_desc("surface", desc);
  reader.read_int("speed", speed);
  reader.read_float("parallax", para);
}

HotspotData::HotspotData (const HotspotData& old) : WorldObjData(old),
						    speed(old.speed),
						    para(old.para),
						    desc(old.desc),
                                                    pos(old.pos)
{
}

void 
HotspotData::insert_WorldObjs (World* world)
{
  world->add_object(new WorldObjs::Hotspot(*this));
}

EditorObjLst 
HotspotData::create_EditorObj ()
{
  return EditorObjLst(1, new EditorObjs::HotspotObj(*this));
}

} //namespace WorldObjsData

/* EOF */
