//  $Id: SwitchDoor.cc,v 1.16 2001/08/07 11:24:40 grumbel Exp $
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

#include "../World.hh"
#include "../PinguHolder.hh"
#include "../PingusResource.hh"
#include "SwitchDoor.hh"

SwitchDoorData::SwitchDoorData ()
{
  door_height = 10;
}

void 
SwitchDoorData::write_xml(std::ofstream* xml)
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

boost::shared_ptr<WorldObjData>
SwitchDoorData::create(xmlDocPtr doc, xmlNodePtr cur)
{
  boost::shared_ptr<SwitchDoorData> data(new SwitchDoorData ());

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
		  data->switch_pos = XMLhelper::parse_vector (doc, subcur);
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
		  data->door_pos = XMLhelper::parse_vector (doc, subcur);
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
  door_tile_cmap  = PingusResource::load_surface("switchdoor_tile_cmap", "worldobjs");
  switch_sur = PingusResource::load_surface("switchdoor_switch", "worldobjs");
  is_opening = false;

  SwitchDoorData* switchdoor = dynamic_cast<SwitchDoorData*>(data);

  door_height = switchdoor->door_height;
  current_door_height = door_height;
  door_pos    = switchdoor->door_pos;
  switch_pos  = switchdoor->switch_pos;
}

SwitchDoor::~SwitchDoor ()
{
}

void 
SwitchDoor::draw_colmap()
{
  world->get_colmap ()->put (door_box, door_pos.x, door_pos.y, GroundpieceData::GP_SOLID);
  for (int i=0; i < door_height; i++)
    world->get_colmap ()->put (door_tile_cmap,
			       door_pos.x + 1, 
			       door_pos.y + i * door_tile.get_height () + door_box.get_height (),
			       GroundpieceData::GP_SOLID);
}

void
SwitchDoor::draw_offset(int x_of, int y_of, float s = 1.0)
{
  door_box.put_screen (door_pos.x+ x_of, door_pos.y+ y_of);
  for (int i=0; i < current_door_height; i++)
    door_tile.put_screen (door_pos.x+ x_of + 1, 
			  door_pos.y+ y_of 
			  + i * door_tile.get_height ()
			  + door_box.get_height ());
  switch_sur.put_screen (switch_pos.x+ x_of, switch_pos.y+ y_of);
}

void
SwitchDoor::update(float delta)
{
  if (current_door_height > 0)
    {
      if (!is_opening)
	{
	  // Check if a pingu is passing the switch
	  PinguHolder* holder = world->get_pingu_p();
      
	  for (PinguIter pingu = holder->begin (); pingu != holder->end (); pingu++)
	    {
	      if ((*pingu)->get_x()    > switch_pos.x
		  && (*pingu)->get_x() < switch_pos.x + (int) switch_sur.get_width ()
		  && (*pingu)->get_y() > switch_pos.y
		  && (*pingu)->get_y() < switch_pos.y + (int) switch_sur.get_height ())
		{
		  is_opening = true;
		}
	    }
	}
      else
	{
	  // Open the door
	  current_door_height -= 1;

	  // If the door is opend enough, so that a pingus fits under
	  // it, we remove the door from the colmap
	  if (current_door_height + 10 < door_height)
	    {
	      world->get_colmap ()->put (door_box, door_pos.x, door_pos.y, GroundpieceData::GP_NOTHING);
	      for (int i=0; i < door_height; i++)
		world->get_colmap ()->put (door_tile_cmap,
					   door_pos.x+ 1, 
					   door_pos.y+ i * door_tile.get_height () + door_box.get_height (),
					   GroundpieceData::GP_NOTHING);
	    }
	}
    }
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

  width  = surf.get_width ();
  height = surf.get_height ();
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

  width  = door_box.get_width ();
  height = (door_tile.get_height () * door_height) + door_box.get_height ();
  
  position = &door_pos;
}

EditorSwitchDoorObj::~EditorSwitchDoorObj ()
{
}

std::list<boost::shared_ptr<EditorObj> > 
EditorSwitchDoorObj::create (WorldObjData* obj)
{
  std::list<boost::shared_ptr<EditorObj> > objs;
  boost::shared_ptr<EditorSwitchDoorObj> switchdoor_obj (new EditorSwitchDoorObj (obj));

  objs.push_back (switchdoor_obj);
  objs.push_back (boost::shared_ptr<EditorObj>(new EditorSwitchDoorSwitchObj (switchdoor_obj.get())));

  return objs;
}

/** Create this object (and child objects) with resonable defaults
    for the editor */
std::list<boost::shared_ptr<EditorObj> >
EditorSwitchDoorObj::create (const CL_Vector& pos)
{
  SwitchDoorData data;

  data.door_pos = pos;
  data.switch_pos = pos;
  data.door_height = 15;

  return EditorObj::create (&data);
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
  sprintf (str, "SwitchDoor - (%f %f %f)", 
	   door_pos.x, door_pos.y, door_pos.z);
  return str;
}

void
EditorSwitchDoorObj::draw (boost::dummy_ptr<EditorView> view)
{
  view->draw_line (door_pos, switch_pos, 1.0, 0.0, 0.0);

  view->draw (door_box, int(door_pos.x + x_of), int(door_pos.y + y_of));

  for (int i = 0; i < door_height; i++)
    {
      view->draw (door_tile, 
		  int(door_pos.x + x_of + 1), 
		  int(door_pos.y + y_of + (i * door_tile.get_height ())
		      + door_box.get_height ()));
    }
}

void
EditorSwitchDoorObj::make_larger ()
{
  door_height += 1;
}

void 
EditorSwitchDoorObj::make_smaller ()
{
  if (door_height > 1)
    door_height -= 1;
}

/* EOF */
