//  $Id: exit_data.cxx,v 1.5 2002/08/23 15:49:48 torangan Exp $
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
#include "exit.hxx"
#include "editor/plfobj.hxx"
#include "string_converter.hxx"
#include "xml_helper.hxx"

ExitData::ExitData (xmlDocPtr doc, xmlNodePtr cur)
{
  clean ();

  char* pos_handling = (char*)xmlGetProp(cur, (xmlChar*)"use-old-pos-handling");
  if (pos_handling)
    {
      std::cout << "XMLPLF: Use Old Pos Handling: " << pos_handling << std::endl;
      use_old_pos_handling = static_cast<bool>(StringConverter::to_int (pos_handling));
      xmlFree (pos_handling);
    }
  else
    {
      use_old_pos_handling = true;
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
	{
	  pos = XMLhelper::parse_vector(doc, cur);
	}
      else if (strcmp((char*)cur->name, "surface") == 0)
	{
	  desc = XMLhelper::parse_surface(doc, cur);
	}
      else if (strcmp((char*)cur->name, "owner-id") == 0)
	{
	  owner_id = XMLhelper::parse_int(doc, cur);
	}
      else
	{
	  std::cout << "XMLPLF: Unhandled exit tag: " << (char*)cur->name << std::endl;
	}
      cur = cur->next;	
    }
}

ExitData::ExitData (const ExitData& old) : WorldObjData(old),
                                           pos(old.pos),
					   desc(old.desc),
					   owner_id(old.owner_id),
					   use_old_pos_handling(old.use_old_pos_handling)
{
}

ExitData
ExitData::operator= (const ExitData& old)
{
  if (this == &old)
    return *this;
    
  WorldObjData::operator=(old);
  
  pos                  = old.pos;
  desc                 = old.desc;
  owner_id             = old.owner_id;
  use_old_pos_handling = old.use_old_pos_handling;
  
  return *this;
}

void 
ExitData::write_xml(std::ostream& xml)
{
  xml << "<exit use-old-pos-handling=\"" << use_old_pos_handling << "\">\n";

  // FIXME: Repair me
  //pos.x += surf.get_width ()/2;
  //pos.y += surf.get_height ();
  XMLhelper::write_vector_xml(xml, pos);
  
  XMLhelper::write_desc_xml(xml, desc);
  xml << "  <owner-id>" << owner_id << "</owner-id>"
      << "</exit>\n"
      << std::endl;
}

WorldObj* 
ExitData::create_WorldObj()
{
  return new Exit (*this);
}

EditorObjLst 
ExitData::create_EditorObj()
{
  EditorObjLst lst;
  lst.push_back (new ExitObj (*this));
  return lst;
}

/* EOF */

