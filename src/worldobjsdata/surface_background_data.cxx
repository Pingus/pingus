//  $Id: surface_background_data.cxx,v 1.6 2003/02/18 01:23:52 grumbel Exp $
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
#include "../xml_helper.hxx"
#include "../xml_file_reader.hxx"
#include "../editorobjs/surface_background_obj.hxx"
#include "../worldobjs/surface_background.hxx"
#include "surface_background_data.hxx"

namespace WorldObjsData {

SurfaceBackgroundData::SurfaceBackgroundData ()
  : para_x(0.5),
    para_y(0.5),
    pos(Vector(0, 0, -150)),
    scroll_x(0.0),
    scroll_y(0.0),
    color(Color(0,0,0,0)),
    stretch_x(false),
    stretch_y(false)
{
}

SurfaceBackgroundData::SurfaceBackgroundData (const SurfaceBackgroundData& old) 
  : WorldObjData(old),
    desc(old.desc),
    para_x(old.para_x),
    para_y(old.para_y),
    pos(old.pos),
    scroll_x(old.scroll_x),
    scroll_y(old.scroll_y),
    color(old.color),
    stretch_x(old.stretch_x),
    stretch_y(old.stretch_y)
{
}

void
SurfaceBackgroundData::write_xml (std::ostream& xml)
{
  xml << "<background type=\"surface\">\n";
  XMLhelper::write_desc_xml(xml, desc);
  
  xml << "  <color>\n"
      << "    <red>"   << color.red   << "</red>\n"
      << "    <green>" << color.green << "</green>\n"
      << "    <blue>"  << color.blue << "</blue>\n"
      << "    <alpha>" << color.alpha   << "</alpha>\n"
      << "  </color>\n"
      << "  <scroll-x>"  << scroll_x << "</scroll-x>\n"
      << "  <scroll-y>"  << scroll_y << "</scroll-y>\n"
      << "  <para-x>"    << para_x << "</para-x>\n"
      << "  <para-y>"    << para_y << "</para-y>\n"
      << "  <stretch-x>" << stretch_x << "</stretch-x>\n"
      << "  <stretch-y>" << stretch_y << "</stretch-y>\n";
  XMLhelper::write_vector_xml(xml, pos);
  xml << "</background>\n"
      << std::endl;
}

SurfaceBackgroundData::SurfaceBackgroundData (xmlDocPtr doc, xmlNodePtr cur)
{
  pos.z = -150;

  XMLFileReader reader(doc, cur);

  reader.read_desc("surface", desc);
  reader.read_color("color", color);

  reader.read_float("para-x", para_x);
  reader.read_float("para-y", para_y);

  reader.read_float("scroll-x", scroll_x);
  reader.read_float("scroll-y", scroll_y);
  
  reader.read_bool("stretch-x", stretch_x);
  reader.read_bool("stretch-y", stretch_y);

  reader.read_vector("position", pos);
}

WorldObj* 
SurfaceBackgroundData::create_WorldObj ()
{
  return new WorldObjs::SurfaceBackground(*this);
}

EditorObjLst 
SurfaceBackgroundData::create_EditorObj ()
{
  return EditorObjLst(1, new EditorObjs::SurfaceBackgroundObj(*this));
}

} // namespace WorldObjsData

/* EOF */
