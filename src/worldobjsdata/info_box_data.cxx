//  $Id: info_box_data.cxx,v 1.6 2002/09/25 17:25:49 grumbel Exp $
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
#include "../pingus_resource.hxx"
#include "../xml_helper.hxx"
#include "../editorobjs/info_box_obj.hxx"
#include "../worldobjs/info_box.hxx"
#include "info_box_data.hxx"

namespace WorldObjsData {

InfoBoxData::InfoBoxData () 
{
}

InfoBoxData::~InfoBoxData ()
{
}

WorldObj* 
InfoBoxData::create_WorldObj ()
{
  return new WorldObjs::InfoBox(this);
}

EditorObjLst
InfoBoxData::create_EditorObj ()
{
  return EditorObjLst(1, new EditorObjs::InfoBoxObj(this));
}

InfoBoxData::InfoBoxData (xmlDocPtr doc, xmlNodePtr cur)
{
  cur = cur->children;
  
  while (cur)
    {
      if (xmlIsBlankNode(cur)) 
	{
	  cur = cur->next;
	  continue;
	}
      else if (XMLhelper::equal_str(cur->name, "position"))
	{
	  pos = XMLhelper::parse_vector (doc, cur);
	}
      else if (XMLhelper::equal_str(cur->name, "info-text"))
	{
	  info_text = XMLhelper::parse_string (doc, cur);
	}
      else
	std::cout << "InfoBox::creata (): Unhandled " << cur->name << std::endl;
      cur = cur->next;
    }
}

InfoBoxData::InfoBoxData (const InfoBoxData& old)
  : WorldObjData(old),
    info_text(old.info_text),
    pos(old.pos),
    text_pos(old.text_pos)
{
}

void 
InfoBoxData::write_xml (std::ostream& xml)
{
  xml << "  <worldobj type=\"infobox\">\n";
  XMLhelper::write_vector_xml (xml, pos);
  xml << "   <info-text>" << info_text << "</info-text>\n" 
      << "  </worldobj>\n" << std::endl;
}

} // namespace WorldObjsData

/* EOF */
