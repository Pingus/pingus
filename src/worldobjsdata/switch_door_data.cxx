//  $Id: switch_door_data.cxx,v 1.2 2002/09/14 19:06:35 torangan Exp $
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
#include "../pingus_resource.hxx"
#include "../xml_helper.hxx"
#include "../editorobjs/switch_door_obj.hxx"
#include "../worldobjs/switch_door.hxx"
#include "switch_door_data.hxx"

namespace WorldObjsData {

SwitchDoorData::SwitchDoorData () 
  : door_box      (PingusResource::load_surface("switchdoor_box"      , "worldobjs")),
    door_tile     (PingusResource::load_surface("switchdoor_tile"     , "worldobjs")),
    door_tile_cmap(PingusResource::load_surface("switchdoor_tile_cmap", "worldobjs")),
    switch_sur    (PingusResource::load_surface("switchdoor_switch"   , "worldobjs")),
    door_height(10)
{
}

SwitchDoorData::SwitchDoorData (xmlDocPtr doc, xmlNodePtr cur)
{
  cur = cur->children;
  
  while (cur)
    {
      if (xmlIsBlankNode(cur)) {
	cur = cur->next;
	continue;
      }
      
      if (XMLhelper::equal_str(cur->name, "switch"))
	{
	  xmlNodePtr subcur = cur->children;
  
	  while (subcur)
	    {
	      if (xmlIsBlankNode(subcur)) {
		subcur = subcur->next;
		continue;
	      }
	      
	      if (XMLhelper::equal_str(subcur->name, "position"))
		{
		  switch_pos = XMLhelper::parse_vector(doc, subcur);
		}
	      else
		std::cout << "SwitchDoorData: switch: Unhandled " << subcur->name << std::endl;

	      subcur = subcur->next;
	    }
	}
      else if (XMLhelper::equal_str(cur->name, "door"))
	{
	  xmlNodePtr subcur = cur->children;

	  while (subcur)
	    {
	      if (xmlIsBlankNode(subcur)) {
		subcur = subcur->next;
		continue;
	      }
	      
	      if (XMLhelper::equal_str(subcur->name, "position"))
		{
		  door_pos = XMLhelper::parse_vector(doc, subcur);
		}
	      else if (XMLhelper::equal_str(subcur->name, "height"))
		{
		  door_height = XMLhelper::parse_int(doc, subcur);
		}
	      else
		std::cout << "SwitchDoor::door: Unhandled " << subcur->name << std::endl;

	      subcur = subcur->next;
	    }
	}
      cur = cur->next;
    }
}

SwitchDoorData::SwitchDoorData (const SwitchDoorData& old) 
                              : WorldObjData(old),
			        door_box(old.door_box),
				door_tile(old.door_tile),
				door_tile_cmap(old.door_tile_cmap),
				switch_sur(old.switch_sur),
			        door_pos(old.door_pos),
				switch_pos(old.switch_pos),
				door_height(old.door_height)
{
}

void 
SwitchDoorData::write_xml (std::ostream& xml)
{
  xml << "  <worldobj type=\"switchdoor\">\n";
  xml << "    <switch>\n";
  XMLhelper::write_vector_xml(xml, switch_pos);
  xml << "    </switch>\n"
      << "    <door>\n"
      << "    <height>\n" << door_height << "</height>\n";
  XMLhelper::write_vector_xml(xml, door_pos);
  xml << "    </door>\n"
      << "  </worldobj>\n" << std::endl;
}

/** Create an WorldObj from the given data object */
WorldObj* 
SwitchDoorData::create_WorldObj ()
{
  return new WorldObjs::SwitchDoor (this);
}

/** Create an EditorObj from the given data object */
EditorObjLst
SwitchDoorData::create_EditorObj ()
{
  EditorObjLst lst(2); 
  EditorObjs::SwitchDoorObj* obj = new EditorObjs::SwitchDoorObj(this);
  lst[0] = obj;
  lst[1] = new EditorObjs::SwitchDoorSwitchObj(obj);
  return lst;
}

} // namespace WorldObjsData

/* EOF */
