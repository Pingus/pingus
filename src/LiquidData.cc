//  $Id: LiquidData.cc,v 1.7 2002/06/08 20:19:53 torangan Exp $
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

#include "boost/smart_ptr.hpp"
#include "Liquid.hh"
#include "editor/PLFObj.hh"
#include "StringConverter.hh"
#include "LiquidData.hh"
#include "XMLhelper.hh"
#include "Position.hh"

void 
LiquidData::write_xml(std::ofstream* xml)
{
  std::cout << "LiquidData::write_xml(std::ofstream* xml)" << std::endl;
  (*xml) << "<liquid use-old-width-handling=\"" << int(old_width_handling) << "\">\n";
  XMLhelper::write_desc_xml(xml, desc);
  XMLhelper::write_position_xml(xml, pos);
  (*xml) << "  <width>" << width << "</width>\n"
	 << "  <speed>" << speed << "</speed>\n"
	 << "</liquid>\n" << std::endl;
}

boost::shared_ptr<WorldObjData> 
LiquidData::create(xmlDocPtr doc, xmlNodePtr cur)
{
  std::cout << "LiquidData::create(xmlDocPtr doc, xmlNodePtr cur)" << std::endl;
  LiquidData* liquid = new LiquidData ();
  
  char* width_handling = (char*)xmlGetProp(cur, (xmlChar*)"use-old-width-handling");
  if (width_handling)
    {
      std::cout << "XMLPLF: Use Old Width Handling: " << width_handling << std::endl;
      liquid->old_width_handling 
	= static_cast<bool>(StringConverter::to_int (width_handling));
      free (width_handling);
    }
  else
    {
      liquid->old_width_handling = true;
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
	liquid->pos = XMLhelper::parse_vector(doc, cur);
      else if (strcmp((char*)cur->name, "surface") == 0)
	liquid->desc = XMLhelper::parse_surface(doc, cur);
      else if (strcmp((char*)cur->name, "speed") == 0)
	liquid->speed = XMLhelper::parse_int(doc, cur);
      else if (strcmp((char*)cur->name, "width") == 0)
	liquid->width = XMLhelper::parse_int(doc, cur);
      else
	{
	  std::cout << "XMLPLF::parse_liquid: Unhandled: " << cur->name << std::endl;
	}	

      cur = cur->next;
    }

  return boost::shared_ptr<WorldObjData> (liquid);
}

boost::shared_ptr<WorldObj> 
LiquidData::create_WorldObj ()
{
  std::cout << "LiquidData::create_WorldObj ()" << std::endl;
  return boost::shared_ptr<WorldObj>(new Liquid (*this));
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
