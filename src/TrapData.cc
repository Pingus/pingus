//  $Id: TrapData.cc,v 1.2 2000/12/16 23:11:20 grumbel Exp $
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

#include "XMLhelper.hh"
#include "TrapData.hh"

void
TrapData::write_xml(ofstream* xml)
{
  (*xml) << "<trap>\n"
	 << "  <type>" << type << "</type>\n";
  XMLhelper::write_position_xml(xml, pos);
  (*xml) << "</trap>\n"
	 << std::endl;
}

boost::shared_ptr<WorldObjData>
TrapData::create(xmlDocPtr doc, xmlNodePtr cur)
{
  boost::shared_ptr<TrapData> trap(new TrapData);
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
	  if (name)
	    {
	      // std::cout << "parse_trap: name = " << name << std::endl;
	      trap->type = name;
	      free(name);
	    }
	} 
      else if (strcmp((char*)cur->name, "position") == 0) 
	{
	  trap->pos = XMLhelper::parse_position(doc, cur);
	}

      cur = cur->next;
    }
  return trap;
}

/* EOF */
