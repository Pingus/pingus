//  $Id: thunderstorm_background_data.cxx,v 1.1 2002/09/18 15:00:37 torangan Exp $
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

#include <ostream>
#include "../xml_helper.hxx"
#include "../editorobjs/thunderstorm_background_obj.hxx"
#include "../worldobjs/thunderstorm_background.hxx"
#include "thunderstorm_background_data.hxx"

namespace WorldObjsData {

void
ThunderstormBackgroundData::write_xml (std::ostream& xml)
{
  xml << "<background type=\"thunderstorm\">\n"
      << "</background>" << std::endl;
}

ThunderstormBackgroundData::ThunderstormBackgroundData (const ThunderstormBackgroundData& old)
  : WorldObjData(old),
    pos(old.pos)
{
}

ThunderstormBackgroundData::ThunderstormBackgroundData (xmlDocPtr doc, xmlNodePtr cur)
{
  cur = cur->children; 
  while (cur)
    {
      if (xmlIsBlankNode(cur)) {
	cur = cur->next;
	continue;
      }

      if (XMLhelper::equal_str(cur->name, "position")) {
	pos = XMLhelper::parse_vector (doc, cur);
      } else {
	std::cout << "ThunderstormBackgroundData::create(xmlDocPtr doc, xmlNodePtr cur) error" << std::endl;
      }
    }
}

WorldObj* 
ThunderstormBackgroundData::create_WorldObj ()
{
  return new WorldObjs::ThunderstormBackground(this);
}

EditorObjLst 
ThunderstormBackgroundData::create_EditorObj ()
{
  return EditorObjLst(1, new EditorObjs::ThunderstormBackgroundObj(this));
}

} // namespace WorldObjsData

/* EOF */
