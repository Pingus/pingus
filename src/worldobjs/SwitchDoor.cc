//  $Id: SwitchDoor.cc,v 1.2 2000/12/04 23:12:13 grumbel Exp $
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

#include "../PingusResource.hh"
#include "SwitchDoor.hh"

SwitchDoorData::SwitchDoorData ()
{
  door_height = 10;
}

void 
SwitchDoorData::write_xml(ofstream* xml)
{
  (*xml) << "  <worldobj type=\"switchdoor\">\n";
  (*xml) << "    <switch>\n";
  XMLhelper::write_position_xml (xml, switch_pos);
  (*xml) << "    </switch>\n"
	 << "    <door>\n"
	 << "    <height>\n" << door_height << "</height>\n";
  XMLhelper::write_position_xml (xml, door_pos);
  (*xml) << "    </door>\n"
	 << "  </worldobj>\n" << std::endl;
}

WorldObjData* 
SwitchDoorData::create(xmlDocPtr doc, xmlNodePtr cur)
{
  SwitchDoorData* data = new SwitchDoorData ();

  cur = cur->children;
  
  while (cur != NULL)
    {
      if (xmlIsBlankNode(cur)) {
	cur = cur->next;
	continue;
      }
      
      if (strcmp((char*)cur->name, "switch") == 0)
	{
	  xmlNodePtr subcur = cur->children;
  
	  while (subcur != NULL)
	    {
	      if (xmlIsBlankNode(subcur)) {
		subcur = subcur->next;
		continue;
	      }
	      
	      if (strcmp((char*)subcur->name, "position") == 0)
		{
		  data->switch_pos = XMLhelper::parse_position (doc, subcur);
		}
	      else
		std::cout << "SwitchDoorData: switch: Unhandled " << subcur->name << std::endl;

	      subcur = subcur->next;
	    }
	}
      else if (strcmp((char*)cur->name, "door") == 0)
	{
	  xmlNodePtr subcur = cur->children;

	  while (subcur != NULL)
	    {
	      if (xmlIsBlankNode(subcur)) {
		subcur = subcur->next;
		continue;
	      }
	      
	      if (strcmp((char*)subcur->name, "position") == 0)
		{
		  data->door_pos = XMLhelper::parse_position (doc, subcur);
		}
	      else if (strcmp((char*)subcur->name, "height") == 0)
		{
		  data->door_height = XMLhelper::parse_int (doc, subcur);
		}
	      else
		std::cout << "SwitchDoor::door: Unhandled " << subcur->name << std::endl;

	      subcur = subcur->next;
	    }
	}
      cur = cur->next;
    }
  
  return data;
}

////////////////
// SwitchDoor //
////////////////

SwitchDoor::SwitchDoor (WorldObjData* data)
{
  door_box   = PingusResource::load_surface("switchdoor_box", "worldobjs");
  door_tile  = PingusResource::load_surface("switchdoor_tile", "worldobjs");
  switch_sur = PingusResource::load_surface("switchdoor_switch", "worldobjs");
  is_open = false;

  SwitchDoorData* switchdoor = dynamic_cast<SwitchDoorData*>(data);

  door_height = switchdoor->door_height;
  door_pos    = switchdoor->door_pos;
  switch_pos  = switchdoor->switch_pos;
}

SwitchDoor::~SwitchDoor ()
{
}

void 
SwitchDoor::draw_colmap()
{
  // not imlp
}

void
SwitchDoor::draw_offset(int x_of, int y_of, float s = 1.0)
{
  door_box->put_screen (door_pos.x_pos + x_of, door_pos.y_pos + y_of);
  for (int i=0; i < door_height; i++)
    door_tile->put_screen (door_pos.x_pos + x_of, door_pos.y_pos + y_of 
			   + i * door_tile->get_height ());
  switch_sur->put_screen (switch_pos.x_pos + x_of, switch_pos.y_pos + y_of);
}

void
SwitchDoor::let_move()
{
}

///////////////////////////////
// EditorSwitchDoorSwitchObj //
///////////////////////////////

EditorSwitchDoorSwitchObj::EditorSwitchDoorSwitchObj (SwitchDoorData* data)
{
  SwitchDoorData* obj = dynamic_cast<SwitchDoorData*>(data);
  assert (obj);

  surf = PingusResource::load_surface ("switchdoor_switch", "worldobjs");

  position = &obj->switch_pos;

  width  = 100;
  height = 100;
}

EditorSwitchDoorSwitchObj::~EditorSwitchDoorSwitchObj ()
{
}

void
EditorSwitchDoorSwitchObj::save_xml (std::ofstream* xml)
{
  // do nothing
}

std::string 
EditorSwitchDoorSwitchObj::status_line()
{
  return "--- EditorSwitchDoorSwitchObj ---";
}

/////////////////////////
// EditorSwitchDoorObj //
/////////////////////////

EditorSwitchDoorObj::EditorSwitchDoorObj (WorldObjData* data)
{
  SwitchDoorData* obj = dynamic_cast<SwitchDoorData*>(data);
  assert (obj);

  door_box   = PingusResource::load_surface("switchdoor_box", "worldobjs");
  door_tile  = PingusResource::load_surface("switchdoor_tile", "worldobjs");

  surf = door_box;

  door_pos  = obj->door_pos;
  switch_pos = obj->switch_pos;
  door_height = obj->door_height;

  width  = 100;
  height = 100;
  
  position = &door_pos;
}

EditorSwitchDoorObj::~EditorSwitchDoorObj ()
{
}

std::list<EditorObj*> 
EditorSwitchDoorObj::create (WorldObjData* obj)
{
  std::list<EditorObj*> objs;
  EditorSwitchDoorObj* switchdoor_obj = new EditorSwitchDoorObj (obj);
  objs.push_back (switchdoor_obj);
  objs.push_back (new EditorSwitchDoorSwitchObj (switchdoor_obj));

  return objs;
}

void
EditorSwitchDoorObj::save_xml (std::ofstream* xml)
{
  write_xml (xml);
}

std::string 
EditorSwitchDoorObj::status_line()
{
  char str[1024];
  sprintf (str, "SwitchDoor - (%d %d %d", 
	   door_pos.x_pos, door_pos.y_pos, door_pos.z_pos);
  return str;
}

void
EditorSwitchDoorObj::draw_offset(int x_of, int y_of)
{
  door_box->put_screen (door_pos.x_pos + x_of, 
			door_pos.y_pos + y_of);

  for (int i = 0; i < door_height; i++)
    door_tile->put_screen (door_pos.x_pos + x_of, 
			   door_pos.y_pos + y_of + i * door_tile->get_height ());
}

/* EOF */
