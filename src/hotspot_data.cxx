//  $Id: hotspot_data.cxx,v 1.4 2002/08/23 15:49:49 torangan Exp $
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

#include <fstream>
#include "editor_hotspot.hxx"
#include "xml_helper.hxx"
#include "hotspot.hxx"

void 
HotspotData::write_xml(std::ostream& xml)
{
  xml << "<hotspot>\n";
  XMLhelper::write_desc_xml(xml, desc);
  XMLhelper::write_vector_xml(xml, pos);
  xml << "  <speed>" << speed << "</speed>\n"
      << "  <parallax>" << para << "</parallax>\n"
      << "</hotspot>\n"
      << std::endl;  
}

HotspotData::HotspotData (xmlDocPtr doc, xmlNodePtr cur)
{
  cur = cur->children;
  while (cur != NULL)
    {
      if (xmlIsBlankNode(cur)) 
	{
	  cur = cur->next;
	  continue;
	}
      
      if (strcmp((char*)cur->name, "surface") == 0)
	{
	  desc = XMLhelper::parse_surface(doc, cur);
	} 
      else if (strcmp((char*)cur->name, "position") == 0) 
	{
	  pos = XMLhelper::parse_vector(doc, cur);
	}
      else if (strcmp((char*)cur->name, "speed") == 0) 
	{
	  speed = XMLhelper::parse_int(doc, cur);
	}
      else if (strcmp((char*)cur->name, "parallax") == 0) 
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
                                                    pos(old.pos),
						    speed(old.speed),
						    para(old.para),
						    desc(old.desc)
{
}

HotspotData
HotspotData::operator= (const HotspotData& old)
{
  if (this == &old)
    return *this;
    
  WorldObjData::operator=(old);
  
  pos   = old.pos;
  speed = old.speed;
  para  = old.para;
  desc  = old.desc;
  
  return *this;
}

WorldObj* 
HotspotData::create_WorldObj()
{
  return new Hotspot (*this);
}

EditorObjLst 
HotspotData::create_EditorObj()
{
  EditorObjLst lst;
  lst.push_back (new EditorHotspot (*this)) ;
  return lst;
}

/* EOF */

