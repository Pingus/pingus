//  $Id: guillotine_data.cxx,v 1.1 2002/09/04 14:55:13 torangan Exp $
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
#include "guillotine_data.hxx"
#include "../xml_helper.hxx"
#include "../editorobjs/guillotine_obj.hxx"
#include "../worldobjs/guillotine.hxx"
#include "../pingus_resource.hxx"

#include <iostream>

namespace WorldObjsData {

  GuillotineData::GuillotineData ()
  {
  }

  GuillotineData::GuillotineData (xmlDocPtr doc, xmlNodePtr cur) 
                                : surface  (PingusResource::load_surface("Traps/guillotinekill", "traps")),
			          idle_surf(PingusResource::load_surface("Traps/guillotineidle", "traps"))
  {
    if (cur->name)
      std::cout << (const char*)cur->name << std::endl;
      
    cur = cur->children;
    while (cur)
      {
        XMLhelper::skip_blank(cur);

        if (! strcmp(reinterpret_cast<const char*>(cur->name), "position")) 
	  {
	    pos = XMLhelper::parse_vector(doc, cur);
	  }
	else
	  std::cout << (const char*)cur->name << std::endl;
	  
	cur = cur->next;
      }
  }

  GuillotineData::GuillotineData (const GuillotineData& old) : WorldObjData(old),
                                                               surface(old.surface),
							       idle_surf(old.idle_surf),	                                                 
							       pos(old.pos)
  {
  }

  void
  GuillotineData::write_xml (std::ostream& xml)
  {
    xml << "<worldobj name=\"guillotine\">\n\t";
    XMLhelper::write_vector_xml(xml, pos);
    xml << "</worldobj>\n\n";
  }

  WorldObj*
  GuillotineData::create_WorldObj ()
  {
    return new WorldObjs::Guillotine(this);
  }

  EditorObjLst
  GuillotineData::create_EditorObj ()
  {
    return EditorObjLst(1, new EditorObjs::GuillotineObj(this));
  }

}

/* EOF */
