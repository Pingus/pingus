//  $Id: conveyor_belt_data.cxx,v 1.4 2002/09/25 17:25:49 grumbel Exp $
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
#include "../editorobjs/conveyor_belt_obj.hxx"
#include "../worldobjs/conveyor_belt.hxx"
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
	  pos = XMLhelper::parse_vector(doc, cur);
	}
      else if (XMLhelper::equal_str(cur->name, "width"))
	{
	  width = XMLhelper::parse_int(doc, cur);
	}
      else if (XMLhelper::equal_str(cur->name, "speed"))
	{
	  speed = XMLhelper::parse_int(doc, cur);
	}
      else
	std::cout << "ConveyorBeltData::create (): Unhandled " << cur->name << std::endl;
      cur = cur->next;
    }
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
  xml << "  <worldobj type=\"conveyorbelt\">";
  XMLhelper::write_vector_xml (xml, pos);
  xml << "    <width>" << width << "</width>\n"
      << "    <speed>" << speed << "</speed>\n"
      << "  </worldobj>\n" << std::endl;
}

WorldObj* 
ConveyorBeltData::create_WorldObj ()
{
  return new WorldObjs::ConveyorBelt(this);
}

EditorObjLst
ConveyorBeltData::create_EditorObj ()
{
  return EditorObjLst(1, new EditorObjs::ConveyorBeltObj(this));
}

} // namespace WorldObjsData

/* EOF */
