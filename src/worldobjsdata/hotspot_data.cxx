//  $Id: hotspot_data.cxx,v 1.1 2002/09/24 09:29:04 torangan Exp $
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
#include "../editorobjs/hotspot_obj.hxx"
#include "../worldobjs/hotspot.hxx"
#include "../xml_helper.hxx"
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
  cur = cur->children;
  while (cur)
    {
      if (xmlIsBlankNode(cur)) 
	{
	  cur = cur->next;
	  continue;
	}
      
      if (XMLhelper::equal_str(cur->name, "surface"))
	{
	  desc = XMLhelper::parse_surface(doc, cur);
	} 
      else if (XMLhelper::equal_str(cur->name, "position")) 
	{
	  pos = XMLhelper::parse_vector(doc, cur);
	}
      else if (XMLhelper::equal_str(cur->name, "speed")) 
	{
	  speed = XMLhelper::parse_int(doc, cur);
	}
      else if (XMLhelper::equal_str(cur->name, "parallax"))
	{
	  para = XMLhelper::parse_int(doc, cur);
	}
      else
	{
	  std::cout << "XMLPLF: parse_hotspot: Unhandled: " << cur->name << std::endl;
	}
      cur = cur->next;
    }
}

HotspotData::HotspotData (const HotspotData& old) : WorldObjData(old),
						    speed(old.speed),
						    para(old.para),
						    desc(old.desc),
                                                    pos(old.pos)
{
}

WorldObj* 
HotspotData::create_WorldObj ()
{
  return new WorldObjs::Hotspot(this);
}

EditorObjLst 
HotspotData::create_EditorObj ()
{
  return EditorObjLst(1, new EditorObjs::HotspotObj(this));
}

} //namespace WorldObjsData

/* EOF */
