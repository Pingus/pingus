//  $Id: ConveyorBelt.cc,v 1.1 2000/11/15 20:57:14 grumbel Exp $
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
  left_sur = PingusResource::load_surface ("conveyorbelt_left", "worldobjs");
  right_sur = PingusResource::load_surface ("conveyorbelt_right", "worldobjs");
  middle_sur = PingusResource::load_surface ("conveyorbelt_middle", "worldobjs");
  
  ConveyorBeltData* obj = dynamic_cast<ConveyorBeltData*>(data);
  
  pos   = obj->pos;
  speed = obj->speed;
  width = obj->width;
  counter = 0;
}

void
ConveyorBelt::draw_offset (int x_of, int y_of, float s = 1.0)
{
  std::cout << "draw ConveyorBelt" << std::endl;

  left_sur->put_screen (pos.x_pos, pos.y_pos, counter);
  for (int i=0; i < width; i++)
    middle_sur->put_screen (pos.x_pos + left_sur->get_width () + i*middle_sur->get_width (), 
			    pos.y_pos, 
			    counter);
  right_sur->put_screen (pos.x_pos + left_sur->get_width () + width*middle_sur->get_width (),
			 pos.y_pos, counter);
}

void 
ConveyorBelt::let_move(void)
{
  std::cout << "let_move ConveyorBelt" << std::endl;
  ++counter;

  if (counter > 2)
    counter = 0;
}

EditorConveyorBeltObj::EditorConveyorBeltObj ()
{
}

EditorConveyorBeltObj::EditorConveyorBeltObj (WorldObjData* obj)
{
  ConveyorBeltData* conveyor_belt = dynamic_cast<ConveyorBeltData*>(obj);

  EditorObj::pos   = conveyor_belt->pos;
  ConveyorBeltData::width = conveyor_belt->width;
  speed = conveyor_belt->speed;
}

EditorConveyorBeltObj::~EditorConveyorBeltObj ()
{
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
