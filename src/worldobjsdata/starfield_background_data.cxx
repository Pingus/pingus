//  $Id: starfield_background_data.cxx,v 1.9 2003/03/05 19:13:59 grumbel Exp $
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
#include "../editorobjs/starfield_background_obj.hxx"
#include "../worldobjs/starfield_background.hxx"
#include "../editor/editorobj_mgr.hxx"
#include "starfield_background_data.hxx"

namespace WorldObjsData {

StarfieldBackgroundData::StarfieldBackgroundData ()
  :  small_stars_count(400),
    middle_stars_count(200),
     large_stars_count(100)
{
}

StarfieldBackgroundData::StarfieldBackgroundData (const StarfieldBackgroundData& old)
  : WorldObjData(old),
    small_stars_count(old. small_stars_count),
    middle_stars_count(old.middle_stars_count),
    large_stars_count(old. large_stars_count)
{
}
 
StarfieldBackgroundData::StarfieldBackgroundData (xmlDocPtr doc, xmlNodePtr cur)
{
  small_stars_count = 100;
  middle_stars_count = 50;
  large_stars_count = 25;


  XMLFileReader reader(doc, cur);
  reader.read_int("small-stars", small_stars_count);
  reader.read_int("middle-stars", middle_stars_count);
  reader.read_int("large-stars", large_stars_count);
}

void 
StarfieldBackgroundData::write_xml(std::ostream& xml)
{
  xml << "<background type=\"starfield\">\n"
      << "  <small-stars  count=\"" <<  small_stars_count << "\"/>\n"
      << "  <middle-stars count=\"" << middle_stars_count << "\"/>\n"
      << "  <large-stars  count=\"" <<  large_stars_count << "\"/>\n"
      << "</background>\n"
      << std::endl;
}

void
StarfieldBackgroundData::insert_WorldObjs (World* world)
{
  world->add_object(new WorldObjs::StarfieldBackground(*this));
}

void
StarfieldBackgroundData::insert_EditorObjs (EditorNS::EditorObjMgr* obj_mgr)
{
  obj_mgr->add(new EditorObjs::StarfieldBackgroundObj(*this));
}

} // namespace WorldObjsData

/* EOF */
