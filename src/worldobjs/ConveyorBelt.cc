//  $Id: ConveyorBelt.cc,v 1.2 2000/11/16 10:23:04 grumbel Exp $
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
#include "../XMLhelper.hh"
#include "ConveyorBelt.hh"

ConveyorBeltData::ConveyorBeltData ()
{
  width = 5;
}

/** Writte the content of this object formated as xml to the given
    stream */
void 
ConveyorBeltData::write_xml(ofstream* xml)
{
  (*xml) << "  <worldobj type=\"iceblock\">";
  XMLhelper::write_position_xml (xml, pos);
  (*xml) << "    <width>" << width << "</width>"
	 << "  </worldobj>" << std::endl;
}

WorldObjData* 
ConveyorBeltData::create(xmlDocPtr doc, xmlNodePtr cur)
{
  ConveyorBeltData* data = new ConveyorBeltData ();

  cur = cur->children;
  
  while (cur != NULL)
    {
      if (xmlIsBlankNode(cur)) 
	{
	  cur = cur->next;
	  continue;
	}
      else if (strcmp((char*)cur->name, "position") == 0)
	{
	  data->pos = XMLhelper::parse_position (doc, cur);
	}
      else if (strcmp((char*)cur->name, "width") == 0)
	{
	  data->width = XMLhelper::parse_int (doc, cur);
	}
      else if (strcmp((char*)cur->name, "speed") == 0)
	{
	  data->speed = XMLhelper::parse_int (doc, cur);
	}
      else
	std::cout << "ConveyorBeltData::creata (): Unhandled " << cur->name << std::endl;
      cur = cur->next;
    }
  
  return data;  
}

/***********************/
/*     ConveyorBelt    */
/***********************/

ConveyorBelt::ConveyorBelt (WorldObjData* data)
{
  left_sur   = PingusResource::load_surface ("conveyorbelt_left", "worldobjs");
  right_sur  = PingusResource::load_surface ("conveyorbelt_right", "worldobjs");
  middle_sur = PingusResource::load_surface ("conveyorbelt_middle", "worldobjs");
  
  ConveyorBeltData* obj = dynamic_cast<ConveyorBeltData*>(data);
  
  pos   = obj->pos;
  speed = obj->speed;
  width = obj->width;
  counter = 0;
  catch_counter = 0;
}

void
ConveyorBelt::draw_offset (int x_of, int y_of, float s = 1.0)
{
  left_sur->put_screen (pos.x_pos + x_of, pos.y_pos + y_of, counter);
  for (int i=0; i < width; i++)
    middle_sur->put_screen (pos.x_pos + left_sur->get_width () + i*middle_sur->get_width () + x_of, 
			    pos.y_pos + y_of, 
			    counter);
  right_sur->put_screen (pos.x_pos + left_sur->get_width () + width*middle_sur->get_width () + x_of,
			 pos.y_pos + y_of, counter);
}

void
ConveyorBelt::draw_colmap ()
{
  CL_Surface* sur = PingusResource::load_surface("conveyorbelt_cmap", "worldobjs");
  for (int i=0; i < (width+2); i++)
    world->get_colmap()->put(sur, pos.x_pos + (15*i), pos.y_pos, GroundpieceData::SOLID);
}

void 
ConveyorBelt::let_move(void)
{
  std::cout << "let_move ConveyorBelt" << std::endl;
  ++counter;

  if (counter > 2)
    counter = 0;

  // Move the Pingus only every second step
  if (catch_counter++ % 2 == 0)
    {
      PinguHolder* holder = world->get_pingu_p();
      for (PinguIter pingu = holder->begin (); pingu != holder->end (); pingu++)
	{
	  if (   (*pingu)->get_x() > pos.x_pos
		 && (*pingu)->get_x() < pos.x_pos + 15*(width+2)
		 && (*pingu)->get_y() > pos.y_pos - 2
		 && (*pingu)->get_y() < pos.y_pos + 10)
	    {
	      (*pingu)->set_pos ((*pingu)->get_x () + 1, (*pingu)->get_y ());
	    }
	}
    }
}

EditorConveyorBeltObj::EditorConveyorBeltObj (WorldObjData* obj)
{
  ConveyorBeltData* conveyor_belt = dynamic_cast<ConveyorBeltData*>(obj);

  pos = conveyor_belt->pos;
  position = &pos;
  ConveyorBeltData::width = conveyor_belt->width;
  speed = conveyor_belt->speed;
  counter = 0;
}

EditorConveyorBeltObj::~EditorConveyorBeltObj ()
{
}

void   
EditorConveyorBeltObj::draw_offset(int, int)
{
  left_sur->put_screen (pos.x_pos + x_of, pos.y_pos + y_of, counter);
  for (int i=0; i < ConveyorBeltData::width; i++)
    middle_sur->put_screen (pos.x_pos + left_sur->get_width () + i*middle_sur->get_width () + x_of, 
			    pos.y_pos + y_of, 
			    counter);
  right_sur->put_screen (pos.x_pos + left_sur->get_width () + ConveyorBeltData::width*middle_sur->get_width () + x_of,
			 pos.y_pos + y_of, counter);
  counter++;
  if (counter > 2)
    counter = 0;
}

void
EditorConveyorBeltObj::draw_scroll_map(int x_pos, int y_pos, int arg_width, int arg_height)
{
  // not supported
}

std::list<EditorObj*> 
EditorConveyorBeltObj::create (WorldObjData* obj)
{
  std::list<EditorObj*> objs;
  objs.push_back (new EditorConveyorBeltObj(obj));
  return objs;
}
  
void
EditorConveyorBeltObj::save_xml (std::ofstream* xml)
{
  write_xml (xml);
}

std::string 
EditorConveyorBeltObj::status_line()
{
  return "ConveyorBelt";
}

/* EOF */
