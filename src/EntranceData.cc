//  $Id: EntranceData.cc,v 1.7 2002/06/08 23:11:07 torangan Exp $
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

#include "entrances/WoodThing.hh"
#include "entrances/Cloud.hh"
#include "editor/PLFObj.hh"
#include "PingusError.hh"
#include "StringConverter.hh"
#include "XMLhelper.hh"

boost::shared_ptr<WorldObjData> 
EntranceData::create(xmlDocPtr doc, xmlNodePtr cur)
{
  EntranceData* entrance = new EntranceData ();
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
	  entrance->type = name;
	  free(name);
	}
      else if (strcmp((char*)cur->name, "owner-id") == 0)
	{
	  entrance->owner_id = XMLhelper::parse_int(doc, cur);
	}
      else if (strcmp((char*)cur->name, "position") == 0)
	{
	  entrance->pos = XMLhelper::parse_vector(doc, cur);
	}
      else if (strcmp((char*)cur->name, "release-rate") == 0)
	{
	  char* release_rate = (char*)xmlNodeListGetString(doc, cur->children, 1);
	  entrance->release_rate = StringConverter::to_int(release_rate);
	  free(release_rate);
	}
      else if (strcmp((char*)cur->name, "direction") == 0)
	{
	  char* direction = (char*)xmlNodeListGetString(doc, cur->children, 1);

	  if (strcmp(direction, "left") == 0)
	    entrance->direction = EntranceData::LEFT;
	  else if (strcmp(direction, "right") == 0)
	    entrance->direction = EntranceData::RIGHT;
	  else if (strcmp(direction, "misc") == 0)
	    entrance->direction = EntranceData::MISC;
	  
	  free(direction);
	}
      else
	{
	  printf("Unhandled: %s\n", (char*)cur->name);
	}	
      cur = cur->next;	
    }
  
  return boost::shared_ptr<WorldObjData>(entrance);
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


boost::shared_ptr<WorldObj> 
EntranceData::create_WorldObj()
{
  if (type == "generic") {
    return boost::shared_ptr<WorldObj>(new Entrance(*this));
  } else if (type == "woodthing") {
    return boost::shared_ptr<WorldObj>(new WoodThing(*this));
  } else if (type == "cloud") {
    return boost::shared_ptr<WorldObj>(new Cloud(*this));
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
