//  $Id: TrapData.cc,v 1.6 2001/08/10 10:56:13 grumbel Exp $
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

#include "traps/smasher.hh"
#include "traps/hammer.hh"
#include "traps/FakeExit.hh"
#include "traps/Guillotine.hh"
#include "traps/LaserExit.hh"
#include "traps/Spike.hh"
#include "traps/Bumper.hh"

#include "PingusError.hh"
#include "Trap.hh"
#include "editor/PLFObj.hh"
#include "XMLhelper.hh"
#include "TrapData.hh"

void
TrapData::write_xml(std::ofstream* xml)
{
  (*xml) << "<worldobj type=" << type << ">\n"
	 << "  <type>" << type << "</type>\n";
  XMLhelper::write_position_xml(xml, pos);
  (*xml) << "</worldobj>\n"
	 << std::endl;
}

boost::shared_ptr<WorldObjData>
TrapData::create(xmlDocPtr doc, xmlNodePtr cur)
{
  boost::shared_ptr<TrapData> trap(new TrapData ());
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
	  trap->pos = XMLhelper::parse_vector(doc, cur);
	}

      cur = cur->next;
    }
  return trap;
}

boost::shared_ptr<WorldObj> 
TrapData::create_WorldObj ()
{
  if (type == "smasher") {
    return boost::shared_ptr<Trap>(new Smasher(*this));
  } else if (type == "hammer") {
    return boost::shared_ptr<Trap>(new Hammer(*this));
  } else if (type == "fake_exit") {
    return boost::shared_ptr<Trap>(new FakeExit(*this));
  } else if (type == "spike") {
    return boost::shared_ptr<Trap>(new Spike(*this));
  } else if (type == "guillotine") {
    return boost::shared_ptr<Trap>(new Guillotine(*this));
  } else if (type == "laser_exit") {
    return boost::shared_ptr<Trap>(new LaserExit(*this));
  } else if (type == "bumper") {
    return boost::shared_ptr<Trap>(new Bumper(*this));
  }
  throw PingusError("Trap: `" + type + "' is unknown");
}

EditorObjLst
TrapData::create_EditorObj () 
{ 
  std::cout << "TrapData::create_EditorObj (): Not implemented" << std::endl;
  EditorObjLst lst; 
  //lst.push_back(boost::shared_ptr<EditorObj> (new EditorSwitchDoorObj));
  return lst;
}

/* EOF */
