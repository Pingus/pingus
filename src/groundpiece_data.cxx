//  $Id: groundpiece_data.cxx,v 1.5 2002/07/02 10:42:38 grumbel Exp $
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
#include "xml_helper.hxx"
#include "editor/editor_groundpiece_obj.hxx"
#include "boost/smart_ptr.hpp"
#include "xml_helper.hxx"

GroundpieceData::GroundpieceData ()
{
  // do nothing
}

GroundpieceData::GroundpieceData (xmlDocPtr doc, xmlNodePtr cur)
{
  gptype = GroundpieceData::GP_GROUND;

  char* gptype_c_str = (char*)xmlGetProp(cur, (xmlChar*)"type");
  if (gptype_c_str)
    {
      gptype = GroundpieceData::string_to_type (gptype_c_str);
      free(gptype_c_str);
    }
  else
    std::cout << "XMLPLF: groundtype empty" << std::endl;

  cur = cur->children;

  while (cur != NULL)
    {
      if (xmlIsBlankNode(cur)) 
	{
	  cur = cur->next;
	  continue;
	}
      
      if (strcmp((char*)cur->name, "position") == 0)
	{
	  pos = XMLhelper::parse_vector(doc, cur);
	}
      else if (strcmp((char*)cur->name, "surface") == 0)
	{
	  desc = XMLhelper::parse_surface(doc, cur);
	}
      else
	{
	  printf("Unhandled: %s\n", (char*)cur->name);
	}
      cur = cur->next;	
    }
}

GroundpieceData::GPType 
GroundpieceData::string_to_type(const std::string& arg_type) 
{
  if (arg_type == "solid")
    return GroundpieceData::GP_SOLID;
  else if (arg_type == "transparent")    
    return GroundpieceData::GP_TRANSPARENT;
  else if (arg_type == "ground")
    return GroundpieceData::GP_GROUND;
  else if (arg_type == "bridge")
    return GroundpieceData::GP_BRIDGE;
  else if (arg_type == "water")
    return GroundpieceData::GP_WATER;
  else if (arg_type == "lava") 
    return GroundpieceData::GP_LAVA;
  else if (arg_type == "remove") 
    return GroundpieceData::GP_REMOVE;
  else
    {
      std::cout << "GroundpieceData: Unhandled type: " << arg_type << std::endl;
      return GroundpieceData::GP_GROUND;
    }
}


std::string 
GroundpieceData::type_to_string(GPType arg_type) 
  {
    switch (arg_type)
      { 
      case GroundpieceData::GP_SOLID:
	return "solid";
      case GroundpieceData::GP_TRANSPARENT:
	return "transparent";
      case GroundpieceData::GP_GROUND:
	return "ground";
      case GroundpieceData::GP_BRIDGE:
	return "bridge";
      case GroundpieceData::GP_WATER:
	return "water";
      case GroundpieceData::GP_LAVA:
	return "lava";
      case GroundpieceData::GP_REMOVE:
	return "remove";
      default:
	std::cout << "GroundpieceData: Unhandled type: " << arg_type << std::endl;
	return "ground";
      }
  }


EditorObjLst
GroundpieceData::create_EditorObj()
{
  EditorObjLst lst;
  lst.push_back(new EditorGroundpieceObj(*this));
  return lst;
}

void
GroundpieceData::write_xml(std::ostream& xml)
{
  xml << "<groundpiece type=\"" << GroundpieceData::type_to_string(gptype) << "\">\n";
  XMLhelper::write_desc_xml(xml, desc);
  XMLhelper::write_vector_xml(xml, pos);
  xml << "</groundpiece>\n" << std::endl;
}

/* EOF */
