//  $Id: solid_color_background_data.cxx,v 1.6 2003/03/04 12:53:47 grumbel Exp $
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
#include "../editorobjs/solid_color_background_obj.hxx"
#include "../worldobjs/solid_color_background.hxx"
#include "../editor/object_manager.hxx"
#include "solid_color_background_data.hxx"

namespace WorldObjsData {

SolidColorBackgroundData::SolidColorBackgroundData (xmlDocPtr doc, xmlNodePtr cur)
{
  XMLFileReader reader(doc, cur);
  reader.read_color("color", color);
}

SolidColorBackgroundData::SolidColorBackgroundData (const SolidColorBackgroundData& old)
  : WorldObjData(old),
    pos(old.pos),
    color(old.color)
{
}

void
SolidColorBackgroundData::write_xml (std::ostream& xml)
{
  XMLFileWriter writer(xml);
  writer.begin_section("worldobj", "type=\"solidcolor-background\"");
  writer.write_color("color", color);
  writer.end_section();
}

void
SolidColorBackgroundData::insert_WorldObjs (World* world)
{
  world->add_object(new WorldObjs::SolidColorBackground(*this));
}

void
SolidColorBackgroundData::insert_EditorObjs(EditorNS::ObjectManager* obj_mgr)
{
  obj_mgr->add(new EditorObjs::SolidColorBackgroundObj(*this));
}

} // namespace WorldObjsData

/* EOF */
