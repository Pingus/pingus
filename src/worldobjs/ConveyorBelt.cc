//  $Id: ConveyorBelt.cc,v 1.19 2001/08/04 12:46:23 grumbel Exp $
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
#include "../XMLhelper.hh"
#include "ConveyorBelt.hh"

ConveyorBeltData::ConveyorBeltData ()
{
  width = 5;
  speed = 2;
}

/** Writte the content of this object formated as xml to the given
    stream */
void 
ConveyorBeltData::write_xml(std::ofstream* xml)
{
  (*xml) << "  <worldobj type=\"conveyorbelt\">";
  XMLhelper::write_position_xml (xml, pos);
  (*xml) << "    <width>" << width << "</width>\n"
	 << "    <speed>" << speed << "</speed>\n"
	 << "  </worldobj>\n" << std::endl;
}

boost::shared_ptr<WorldObjData>
ConveyorBeltData::create(xmlDocPtr doc, xmlNodePtr cur)
{
  boost::shared_ptr<ConveyorBeltData> data(new ConveyorBeltData ());

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
	  data->pos = XMLhelper::parse_vector (doc, cur);
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
	std::cout << "ConveyorBeltData::create (): Unhandled " << cur->name << std::endl;
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
}

void
ConveyorBelt::draw_offset (int x_of, int y_of, float s = 1.0)
{
  left_sur.put_screen (int(pos.x + x_of), int(pos.y + y_of), counter);
  for (int i=0; i < width; i++)
    middle_sur.put_screen (int(pos.x + left_sur.get_width () + i*middle_sur.get_width () + x_of), 
			   int(pos.y + y_of), 
			   counter);
  right_sur.put_screen (int(pos.x + left_sur.get_width () + width*middle_sur.get_width () + x_of),
			int(pos.y + y_of), counter);
}

void
ConveyorBelt::draw_colmap ()
{
  CL_Surface sur(PingusResource::load_surface("conveyorbelt_cmap", "worldobjs"));
  for (int i=0; i < (width+2); i++)
    world->get_colmap()->put(sur, pos.x + (15*i), pos.y, GroundpieceData::GP_SOLID);
}

void 
ConveyorBelt::update(float delta)
{
  counter += speed * delta;

  if (counter >= 14.0f)
    counter = 0.0f;
  else if (counter < 0.0f)
    counter = middle_sur.get_num_frames () - 1;

  PinguHolder* holder = world->get_pingu_p();
  for (PinguIter pingu = holder->begin (); pingu != holder->end (); ++pingu)
    {
      if ((*pingu)->get_x() > pos.x
	  && (*pingu)->get_x() < pos.x + 15*(width+2)
	  && (*pingu)->get_y() > pos.y - 2
	  && (*pingu)->get_y() < pos.y + 10)
	{
	  CL_Vector pos = (*pingu)->get_pos ();
	  pos.x -= speed * delta;
	  (*pingu)->set_pos (pos);
	}
    }
}

EditorConveyorBeltObj::EditorConveyorBeltObj (WorldObjData* obj)
{
  ConveyorBeltData* conveyor_belt = dynamic_cast<ConveyorBeltData*>(obj);

  left_sur   = PingusResource::load_surface ("conveyorbelt_left", "worldobjs");
  right_sur  = PingusResource::load_surface ("conveyorbelt_right", "worldobjs");
  middle_sur = PingusResource::load_surface ("conveyorbelt_middle", "worldobjs");

  EditorObj::width  = left_sur.get_width() + right_sur.get_width()
    + ConveyorBeltData::width * middle_sur.get_width ();
  EditorObj::height = middle_sur.get_height ();

  pos = conveyor_belt->pos;
  position = &pos;
  ConveyorBeltData::width = conveyor_belt->width;
  speed = conveyor_belt->speed;
  ConveyorBeltData::width = conveyor_belt->width;
  counter = 0.0f;
}

EditorConveyorBeltObj::~EditorConveyorBeltObj ()
{
}

boost::shared_ptr<EditorObj> 
EditorConveyorBeltObj::duplicate()
{
  return boost::shared_ptr<EditorObj>(new EditorConveyorBeltObj (this));
}

void
EditorConveyorBeltObj::draw (boost::dummy_ptr<EditorView> view)
{
  view->draw (left_sur, pos, counter);
  for (int i=0; i < ConveyorBeltData::width; i++)
    view->draw (middle_sur, pos.x + left_sur.get_width () + i*middle_sur.get_width (),
		pos.y,
		counter);
  view->draw (right_sur, pos.x + left_sur.get_width () + ConveyorBeltData::width*middle_sur.get_width (),
			pos.y, counter);
  counter += speed;
  if (counter > 14)
    counter = 0;
  else if (counter < 0)
    counter = middle_sur.get_num_frames () - 1;

}

void
EditorConveyorBeltObj::draw_scroll_map(int x_pos, int y_pos, int arg_width, int arg_height)
{
  // not supported
}

std::list<boost::shared_ptr<EditorObj> > 
EditorConveyorBeltObj::create (WorldObjData* obj)
{
  std::list<boost::shared_ptr<EditorObj> > objs;
  objs.push_back (boost::shared_ptr<EditorObj>(new EditorConveyorBeltObj(obj)));
  return objs;
}

std::list<boost::shared_ptr<EditorObj> > 
EditorConveyorBeltObj::create (const CL_Vector& pos)
{
  ConveyorBeltData data;

  data.pos = pos;

  return EditorObj::create (&data);
}
  
void
EditorConveyorBeltObj::save_xml (std::ofstream* xml)
{
  write_xml (xml);
}

std::string 
EditorConveyorBeltObj::status_line()
{
  char str[1024];
  sprintf (str, "ConveyorBelt - (%f, %f, %f) Speed: %f", pos.x, pos.y, pos.z, speed);
  return str;
}

/* EOF */
