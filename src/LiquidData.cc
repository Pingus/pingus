//  $Id: LiquidData.cc,v 1.13 2002/06/12 14:37:36 torangan Exp $
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
#include "Liquid.hh"
#include "editor/PLFObj.hh"
#include "StringConverter.hh"
#include "LiquidData.hh"
#include "XMLhelper.hh"
#include "boost/smart_ptr.hpp"

void 
LiquidData::write_xml(std::ofstream* xml)
{
  std::cout << "LiquidData::write_xml(std::ofstream* xml)" << std::endl;
  (*xml) << "<liquid use-old-width-handling=\"" << int(old_width_handling) << "\">\n";
  XMLhelper::write_desc_xml(xml, desc);
  XMLhelper::write_vector_xml(xml, pos);
  (*xml) << "  <width>" << width << "</width>\n"
	 << "  <speed>" << speed << "</speed>\n"
	 << "</liquid>\n" << std::endl;
}

LiquidData::LiquidData (xmlDocPtr doc, xmlNodePtr cur)
{
  std::cout << "LiquidData::create(xmlDocPtr doc, xmlNodePtr cur)" << std::endl;
  
  char* width_handling = (char*)xmlGetProp(cur, (xmlChar*)"use-old-width-handling");
  if (width_handling)
    {
      std::cout << "XMLPLF: Use Old Width Handling: " << width_handling << std::endl;
      old_width_handling 
	= static_cast<bool>(StringConverter::to_int (width_handling));
      free (width_handling);
    }
  else
    {
      old_width_handling = true;
    }

  cur = cur->children;
  while (cur != NULL)
    {
      if (xmlIsBlankNode(cur)) 
	{
	  cur = cur->next;
	  continue;
	}

      if (strcmp((char*)cur->name, "position") == 0)
	pos = XMLhelper::parse_vector(doc, cur);
      else if (strcmp((char*)cur->name, "surface") == 0)
	desc = XMLhelper::parse_surface(doc, cur);
      else if (strcmp((char*)cur->name, "speed") == 0)
	speed = XMLhelper::parse_int(doc, cur);
      else if (strcmp((char*)cur->name, "width") == 0)
	width = XMLhelper::parse_int(doc, cur);
      else
	{
	  std::cout << "XMLPLF::parse_liquid: Unhandled: " << cur->name << std::endl;
	}	

      cur = cur->next;
    }
}

WorldObj* 
LiquidData::create_WorldObj ()
{
  std::cout << "LiquidData::create_WorldObj ()" << std::endl;
  return new Liquid (*this);
}

EditorObjLst
LiquidData::create_EditorObj () 
{ 
  std::cout << "LiquidData::create_EditorObj ()" << std::endl;
  EditorObjLst lst; 
  lst.push_back (boost::shared_ptr<EditorObj>(new LiquidObj (*this)));
  return lst;
}

/* EOF */

