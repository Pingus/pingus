//  $Id: switch_door.cxx,v 1.9 2002/09/05 11:26:35 grumbel Exp $
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
#include "../col_map.hxx"
#include "../world.hxx"
#include "../pingu_holder.hxx"
#include "../pingus_resource.hxx"
#include "../editor/editor_view.hxx"
#include "../xml_helper.hxx"
#include "switch_door.hxx"
#include "../pingu.hxx"

SwitchDoorData::SwitchDoorData () : door_height(10)
{
}

SwitchDoorData::SwitchDoorData (xmlDocPtr doc, xmlNodePtr cur)
{
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
		  switch_pos = XMLhelper::parse_vector (doc, subcur);
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
		  door_pos = XMLhelper::parse_vector (doc, subcur);
		}
	      else if (strcmp((char*)subcur->name, "height") == 0)
		{
		  door_height = XMLhelper::parse_int (doc, subcur);
		}
	      else
		std::cout << "SwitchDoor::door: Unhandled " << subcur->name << std::endl;

	      subcur = subcur->next;
	    }
	}
      cur = cur->next;
    }
}

SwitchDoorData::SwitchDoorData (const SwitchDoorData& old) : WorldObjData(old),
                                                             door_pos(old.door_pos),
							     switch_pos(old.switch_pos),
							     door_height(old.door_height)
{
}

SwitchDoorData 
SwitchDoorData::operator= (const SwitchDoorData& old)
{
  if (this == &old)
    return *this;
    
  WorldObjData::operator=(old);
  
  door_pos    = old.door_pos;
  switch_pos  = old.switch_pos;
  door_height = old.door_height;
  
  return *this;
}
  
void 
SwitchDoorData::write_xml(std::ostream& xml)
{
  xml << "  <worldobj type=\"switchdoor\">\n";
  xml << "    <switch>\n";
  XMLhelper::write_vector_xml (xml, switch_pos);
  xml << "    </switch>\n"
      << "    <door>\n"
      << "    <height>\n" << door_height << "</height>\n";
  XMLhelper::write_vector_xml (xml, door_pos);
  xml << "    </door>\n"
      << "  </worldobj>\n" << std::endl;
}

/** Create an WorldObj from the given data object */
WorldObj* 
SwitchDoorData::create_WorldObj ()
{
  return new SwitchDoor (*this);
}

/** Create an EditorObj from the given data object */
EditorObjLst
SwitchDoorData::create_EditorObj ()
{
  EditorObjLst lst; 
  EditorSwitchDoorObj* obj = new EditorSwitchDoorObj (*this);
  lst.push_back(obj);
  lst.push_back(new EditorSwitchDoorSwitchObj (obj));
  return lst;
}

////////////////
// SwitchDoor //
////////////////

SwitchDoor::SwitchDoor (const SwitchDoorData& data)
  : SwitchDoorData (data)
{
  door_box   = PingusResource::load_surface("switchdoor_box", "worldobjs");
  door_tile  = PingusResource::load_surface("switchdoor_tile", "worldobjs");
  door_tile_cmap  = PingusResource::load_surface("switchdoor_tile_cmap", "worldobjs");
  switch_sur = PingusResource::load_surface("switchdoor_switch", "worldobjs");
  is_opening = false;

  current_door_height = door_height;
}

void 
SwitchDoor::draw_colmap()
{
  world->get_colmap ()->put (door_box, int(door_pos.x), int(door_pos.y), GroundpieceData::GP_SOLID);
  for (int i=0; i < door_height; ++i)
    world->get_colmap ()->put (door_tile_cmap,
			       int(door_pos.x), 
			       int(door_pos.y) + i * door_tile.get_height () + door_box.get_height (),
			       GroundpieceData::GP_SOLID);
}

void
SwitchDoor::draw (GraphicContext& gc)
{
  gc.draw (door_box, door_pos);
  for (int i=0; i < current_door_height; ++i)
    gc.draw (door_tile, 
	     int(door_pos.x),  
	     int(door_pos.y) + i * door_tile.get_height () + door_box.get_height ());
  gc.draw (switch_sur, switch_pos);
}

void
SwitchDoor::update(float /*delta*/)
{
  if (current_door_height > 0)
    {
      if (!is_opening)
	{
	  // Check if a pingu is passing the switch
	  PinguHolder* holder = world->get_pingu_p();
      
	  for (PinguIter pingu = holder->begin (); pingu != holder->end (); ++pingu)
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
	      world->get_colmap ()->put (door_box, int(door_pos.x), int(door_pos.y), GroundpieceData::GP_NOTHING);
	      for (int i=0; i < door_height; ++i)
		world->get_colmap ()->put (door_tile_cmap,
					   int(door_pos.x), 
					   int(door_pos.y) + i * door_tile.get_height () + door_box.get_height (),
					   GroundpieceData::GP_NOTHING);
	    }
	}
    }
}

///////////////////////////////
// EditorSwitchDoorSwitchObj //
///////////////////////////////

EditorSwitchDoorSwitchObj::EditorSwitchDoorSwitchObj (EditorSwitchDoorObj* data)
  : SpriteEditorObj ("switchdoor_switch", "worldobjs", data->switch_pos),
  door (data)
{
}

std::string 
EditorSwitchDoorSwitchObj::status_line()
{
  return "--- EditorSwitchDoorSwitchObj ---";
}

EditorObj*
EditorSwitchDoorSwitchObj::duplicate() 
{ 
  return door->duplicate (); 
}

/////////////////////////
// EditorSwitchDoorObj //
/////////////////////////

EditorSwitchDoorObj::EditorSwitchDoorObj (const SwitchDoorData& data)
  : SwitchDoorData (data)
{
  door_box   = PingusResource::load_surface("switchdoor_box", "worldobjs");
  door_tile  = PingusResource::load_surface("switchdoor_tile", "worldobjs");
}

EditorObj*
EditorSwitchDoorObj::duplicate()
{
  std::cout << "EditorSwitchDoorObj::duplicate(): not implemented" << std::endl;
  return 0;
}

/** Create this object (and child objects) with resonable defaults
    for the editor */
EditorObjLst
EditorSwitchDoorObj::create (const CL_Vector& pos)
{
  SwitchDoorData data;

  data.door_pos = pos;
  data.switch_pos = pos;
  data.door_height = 15;

  return data.create_EditorObj ();
}

void
EditorSwitchDoorObj::save_xml (std::ostream& xml)
{
  write_xml (xml);
}

std::string 
EditorSwitchDoorObj::status_line()
{
  char str[128];
  snprintf (str, 128, "SwitchDoor - (%f %f %f)", 
	   door_pos.x, door_pos.y, door_pos.z);
  return str;
}

void
EditorSwitchDoorObj::draw (EditorView * view)
{
  view->draw_line (door_pos, switch_pos, 1.0, 0.0, 0.0);

  view->draw (door_box, int(door_pos.x), int(door_pos.y));

  for (int i = 0; i < door_height; ++i)
    {
      view->draw (door_tile, 
		  int(door_pos.x), 
		  int(door_pos.y + (i * door_tile.get_height ())
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

void 
EditorSwitchDoorObj::set_position_offset(const CL_Vector& offset)
{
  door_pos += offset;
}

/* EOF */
