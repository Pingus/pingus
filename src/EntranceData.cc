//  $Id: EntranceData.cc,v 1.11 2002/06/11 18:28:31 torangan Exp $
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
#include "entrances/WoodThing.hh"
#include "entrances/Cloud.hh"
#include "editor/PLFObj.hh"
#include "PingusError.hh"
#include "StringConverter.hh"
#include "XMLhelper.hh"

EntranceData::EntranceData (xmlDocPtr doc, xmlNodePtr cur)
{
  clean ();

  cur = cur->children;  
  while (cur != NULL)
    {
      if (xmlIsBlankNode(cur)) 
	{
	  cur = cur->next;
	  continue;
	}

      if (strcmp((char*)cur->name, "type") == 0)
	{
	  char* name = (char*)xmlNodeListGetString(doc, cur->children, 1); 
	  type = name;
	  free(name);
	}
      else if (strcmp((char*)cur->name, "owner-id") == 0)
	{
	  owner_id = XMLhelper::parse_int(doc, cur);
	}
      else if (strcmp((char*)cur->name, "position") == 0)
	{
	  pos = XMLhelper::parse_vector(doc, cur);
	}
      else if (strcmp((char*)cur->name, "release-rate") == 0)
	{
	  char* release_rate_str = (char*)xmlNodeListGetString(doc, cur->children, 1);
	  release_rate = StringConverter::to_int(release_rate_str);
	  free(release_rate_str);
	}
      else if (strcmp((char*)cur->name, "direction") == 0)
	{
	  char* direction_str = (char*)xmlNodeListGetString(doc, cur->children, 1);

	  if (strcmp(direction_str, "left") == 0)
	    direction = EntranceData::LEFT;
	  else if (strcmp(direction_str, "right") == 0)
	    direction = EntranceData::RIGHT;
	  else if (strcmp(direction_str, "misc") == 0)
	    direction = EntranceData::MISC;
	  
	  free(direction_str);
	}
      else
	{
	  printf("Unhandled: %s\n", (char*)cur->name);
	}	
      cur = cur->next;	
    }
}

void 
EntranceData::write_xml(std::ofstream *xml)
{
  std::string dir_str;
  
  switch(direction)
    {
    case EntranceData::LEFT:
      dir_str = "left";
      break;
    case EntranceData::RIGHT:
      dir_str = "right";
      break;
    case EntranceData::MISC:
    default:
      dir_str = "misc";
      break;
    }

  (*xml) << "<entrance>\n";
  XMLhelper::write_vector_xml(xml, pos);
  (*xml) << "  <type>" << type << "</type>\n"
	 << "  <direction>" << dir_str << "</direction>\n"
	 << "  <release-rate>" << release_rate << "</release-rate>\n"
	 << "  <owner-id>" << owner_id << "</owner-id>\n"
	 << "</entrance>\n"
	 << std::endl;  
}


WorldObj* 
EntranceData::create_WorldObj()
{
  if (type == "generic") {
    return new Entrance(*this);
  } else if (type == "woodthing") {
    return new WoodThing(*this);
  } else if (type == "cloud") {
    return new Cloud(*this);
  } else {
    throw PingusError("Entrance: Entrance type in PLF file is unknown: " + type);
  }
}

EditorObjLst 
EntranceData::create_EditorObj()
{
  EditorObjLst lst;
  lst.push_back (boost::shared_ptr<EditorObj> (new EntranceObj (*this)));
  return lst;
}

/* EOF */

