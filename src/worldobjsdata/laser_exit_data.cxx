//  $Id: laser_exit_data.cxx,v 1.1 2002/09/04 14:55:13 torangan Exp $
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
#include "laser_exit_data.hxx"
#include "../xml_helper.hxx"
#include "../editorobjs/laser_exit_obj.hxx"
#include "../worldobjs/laser_exit.hxx"
#include "../pingus_resource.hxx"

namespace WorldObjsData {

  LaserExitData::LaserExitData ()
  {
  }

  LaserExitData::LaserExitData (xmlDocPtr doc, xmlNodePtr cur) : surface(PingusResource::load_surface("Traps/laser_exit", "traps"))
  {
    cur = cur->children;
    while (cur)
      {
        XMLhelper::skip_blank(cur);

        if (strcmp(reinterpret_cast<const char*>(cur->name), "position") == 0) 
	  {
	    pos = XMLhelper::parse_vector(doc, cur);
	  }
	  
	cur = cur->next;
      }
  }

  LaserExitData::LaserExitData (const LaserExitData& old) : WorldObjData(old),
                                                            surface(old.surface),
	   					            pos(old.pos)
  {
  }

  void
  LaserExitData::write_xml (std::ostream& xml)
  {
    xml << "<worldobj name=\"laser_exit\">\n\t";
    XMLhelper::write_vector_xml(xml, pos);
    xml << "</worldobj>\n\n";
  }

  WorldObj*
  LaserExitData::create_WorldObj ()
  {
    return new WorldObjs::LaserExit(this);
  }

  EditorObjLst
  LaserExitData::create_EditorObj ()
  {
    return EditorObjLst(1, new EditorObjs::LaserExitObj(this));
  }

}

/* EOF */
