//  $Id: trap_data.cxx,v 1.6 2002/08/23 15:49:51 torangan Exp $
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
#include "traps/smasher.hxx"
#include "traps/hammer.hxx"
#include "traps/fake_exit.hxx"
#include "traps/guillotine.hxx"
#include "traps/laser_exit.hxx"
#include "traps/spike.hxx"
#include "traps/bumper.hxx"

#include "pingus_error.hxx"
#include "editor/plfobj.hxx"
#include "xml_helper.hxx"

TrapData::TrapData ()
{
}

TrapData::TrapData (xmlDocPtr doc, xmlNodePtr cur)
{
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
	      type = name;
	      xmlFree(name);
	    }
	} 
      else if (strcmp((char*)cur->name, "position") == 0) 
	{
	  pos = XMLhelper::parse_vector(doc, cur);
	}

      cur = cur->next;
    }
}

TrapData::TrapData (const TrapData& old) : WorldObjData(old),
                                           type(old.type),
                                           pos(old.pos),
					   x_target(old.x_target),
					   y_target(old.y_target)
{
}

TrapData
TrapData::operator= (const TrapData& old)
{
  if (this == &old)
    return *this;
    
  WorldObjData::operator=(old);
  
  type     = old.type;
  pos      = old.pos;
  x_target = old.x_target;
  y_target = old.y_target;
  
  return *this;
}

void
TrapData::write_xml(std::ostream& xml)
{
  xml << "<trap>\n"
      << "  <type>" << type << "</type>\n";
  XMLhelper::write_vector_xml(xml, pos);
  xml << "</trap>\n"
      << std::endl;
}

WorldObj* 
TrapData::create_WorldObj ()
{
  if (type == "smasher") {
    return new Smasher(*this);
  } else if (type == "hammer") {
    return new Hammer(*this);
  } else if (type == "fake_exit") {
    return new FakeExit(*this);
  } else if (type == "spike") {
    return new Spike(*this);
  } else if (type == "guillotine") {
    return new Guillotine(*this);
  } else if (type == "laser_exit") {
    return new LaserExit(*this);
  } else if (type == "bumper") {
    return new Bumper(*this);
  }
  PingusError::raise("Trap: `" + type + "' is unknown");
  
  return 0; // never reached;
}

EditorObjLst
TrapData::create_EditorObj () 
{ 
  EditorObjLst lst; 
  lst.push_back(new TrapObj(*this));
  return lst;
}

/* EOF */
