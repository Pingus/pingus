//  $Id: conveyor_belt_data.cxx,v 1.10 2003/03/04 12:53:47 grumbel Exp $
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
#include "../xml_file_reader.hxx"
#include "../xml_file_writer.hxx"
#include "../editorobjs/conveyor_belt_obj.hxx"
#include "../worldobjs/conveyor_belt.hxx"
#include "../editor/object_manager.hxx"
#include "conveyor_belt_data.hxx"

namespace WorldObjsData {

ConveyorBeltData::ConveyorBeltData () 
  : width(5),
    speed(2),
    counter(0)
{
}

ConveyorBeltData::ConveyorBeltData (xmlDocPtr doc, xmlNodePtr cur)
{
  XMLFileReader reader(doc, cur);
  reader.read_vector("position", pos);
  reader.read_int("width", width);
  reader.read_float("speed", speed);
}

ConveyorBeltData::ConveyorBeltData (const ConveyorBeltData& old)
  : WorldObjData(old),
    pos(old.pos),
    width(old.width),
    speed(old.speed),
    counter(old.counter)
{
}

/** Writte the content of this object formated as xml to the given
    stream */
void 
ConveyorBeltData::write_xml (std::ostream& xml)
{
  XMLFileWriter writer(xml);
  writer.begin_section("worldobj", "type=\"conveyorbelt\"");
  writer.write_vector("position", pos);
  writer.write_int("width", width);
  writer.write_float("speed", speed);
  writer.end_section();
}

void 
ConveyorBeltData::insert_WorldObjs (World* world)
{
  world->add_object(new WorldObjs::ConveyorBelt(*this));
}

void
ConveyorBeltData::insert_EditorObjs (EditorNS::ObjectManager* obj_mgr)
{
  obj_mgr->add(new EditorObjs::ConveyorBeltObj(*this));
}

} // namespace WorldObjsData

/* EOF */
