//  $Id: ExitData.cc,v 1.11 2002/06/09 13:03:11 grumbel Exp $
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

#include "Exit.hh"
#include "editor/PLFObj.hh"
#include "StringConverter.hh"
#include "XMLhelper.hh"

ExitData::ExitData (xmlDocPtr doc, xmlNodePtr cur)
{
  char* pos_handling = (char*)xmlGetProp(cur, (xmlChar*)"use-old-pos-handling");
  if (pos_handling)
    {
      std::cout << "XMLPLF: Use Old Pos Handling: " << pos_handling << std::endl;
      use_old_pos_handling = static_cast<bool>(StringConverter::to_int (pos_handling));
      free (pos_handling);
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

void 
ExitData::write_xml(std::ofstream* xml)
{
  (*xml) << "<exit use-old-pos-handling=\"" << use_old_pos_handling << "\">\n";

  // FIXME: Repair me
  //pos.x += surf.get_width ()/2;
  //pos.y += surf.get_height ();
  XMLhelper::write_vector_xml(xml, pos);
  
  XMLhelper::write_desc_xml(xml, desc);
  (*xml) << "  <owner-id>" << owner_id << "</owner-id>"
	 << "</exit>\n"
	 << std::endl;
}

boost::shared_ptr<WorldObj> 
ExitData::create_WorldObj()
{
  return boost::shared_ptr<WorldObj> (new Exit (*this));
}

EditorObjLst 
ExitData::create_EditorObj()
{
  EditorObjLst lst;
  lst.push_back (boost::shared_ptr<EditorObj> (new ExitObj (*this)));
  return lst;
}

/* EOF */
