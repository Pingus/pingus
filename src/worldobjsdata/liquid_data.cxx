//  $Id: liquid_data.cxx,v 1.2 2002/09/25 18:09:57 torangan Exp $
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
#include "../editorobjs/liquid_obj.hxx"
#include "../worldobjs/liquid.hxx"
#include "../string_converter.hxx"
#include "../xml_helper.hxx"
#include "liquid_data.hxx"

namespace WorldObjsData {

LiquidData::LiquidData () : old_width_handling(true),
                            width(0),
			    speed(0)
{
}

LiquidData::LiquidData (xmlDocPtr doc, xmlNodePtr cur)
{
  std::cout << "LiquidData::create(xmlDocPtr doc, xmlNodePtr cur)" << std::endl;
  
  char* width_handling = XMLhelper::get_prop(cur, "use-old-width-handling");
  if (width_handling)
    {
      std::cout << "XMLPLF: Use Old Width Handling: " << width_handling << std::endl;
      old_width_handling = StringConverter::to_int(width_handling);
      xmlFree(width_handling);
    }

  cur = cur->children;
  while (cur)
    {
      if (xmlIsBlankNode(cur)) 
	{
	  cur = cur->next;
	  continue;
	}

      if (XMLhelper::equal_str(cur->name, "position"))
	pos = XMLhelper::parse_vector(doc, cur);
      else if (XMLhelper::equal_str(cur->name, "surface"))
	desc = XMLhelper::parse_surface(doc, cur);
      else if (XMLhelper::equal_str(cur->name, "speed"))
	speed = XMLhelper::parse_int(doc, cur);
      else if (XMLhelper::equal_str(cur->name, "width"))
	width = XMLhelper::parse_int(doc, cur);
      else
	{
	  std::cout << "XMLPLF::parse_liquid: Unhandled: " << cur->name << std::endl;
	}	

      cur = cur->next;
    }
}

LiquidData::LiquidData (const LiquidData& old) : WorldObjData(old),
                                                 desc(old.desc),
						 pos(old.pos),
						 old_width_handling(old.old_width_handling),
						 width(old.width),
						 speed(old.speed)
{
}


void 
LiquidData::write_xml (std::ostream& xml)
{
  std::cout << "LiquidData::write_xml(std::ostream& xml)" << std::endl;
  xml << "<liquid use-old-width-handling=\"" << old_width_handling << "\">\n";
  XMLhelper::write_desc_xml(xml, desc);
  XMLhelper::write_vector_xml(xml, pos);
  xml << "  <width>" << width << "</width>\n"
      << "  <speed>" << speed << "</speed>\n"
      << "</liquid>\n" << std::endl;
}

WorldObj* 
LiquidData::create_WorldObj ()
{
  std::cout << "LiquidData::create_WorldObj ()" << std::endl;
  return new WorldObjs::Liquid(this);
}

EditorObjLst
LiquidData::create_EditorObj () 
{ 
  std::cout << "LiquidData::create_EditorObj ()" << std::endl;
  return EditorObjLst(1, new EditorObjs::LiquidObj(this));
}

} // namespace WorldObjsData

/* EOF */
