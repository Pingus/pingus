//  $Id: conveyor_belt.cxx,v 1.8 2002/09/04 14:55:13 torangan Exp $
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
#include "../editor/editor_view.hxx"
#include "../world.hxx"
#include "../pingu_holder.hxx"
#include "../pingus_resource.hxx"
#include "../xml_helper.hxx"
#include "conveyor_belt.hxx"
#include "../pingu.hxx"

ConveyorBeltData::ConveyorBeltData () : width(5), speed(2)
{
}

ConveyorBeltData::ConveyorBeltData (xmlDocPtr doc, xmlNodePtr cur)
{
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
	  pos = XMLhelper::parse_vector (doc, cur);
	}
      else if (strcmp((char*)cur->name, "width") == 0)
	{
	  width = XMLhelper::parse_int (doc, cur);
	}
      else if (strcmp((char*)cur->name, "speed") == 0)
	{
	  speed = XMLhelper::parse_int (doc, cur);
	}
      else
	std::cout << "ConveyorBeltData::create (): Unhandled " << cur->name << std::endl;
      cur = cur->next;
    }
}

ConveyorBeltData::ConveyorBeltData (const ConveyorBeltData& old) : WorldObjData(old),
                                                                   pos(old.pos),
								   width(old.width),
								   speed(old.speed)
{
}

ConveyorBeltData
ConveyorBeltData::operator= (const ConveyorBeltData& old)
{
  if (this == &old)
    return *this;
    
  WorldObjData::operator=(old);
  
  pos   = old.pos;
  width = old.width;
  speed = old.speed;
  
  return *this;
}

/** Writte the content of this object formated as xml to the given
    stream */
void 
ConveyorBeltData::write_xml(std::ostream& xml)
{
  xml << "  <worldobj type=\"conveyorbelt\">";
  XMLhelper::write_vector_xml (xml, pos);
  xml << "    <width>" << width << "</width>\n"
      << "    <speed>" << speed << "</speed>\n"
      << "  </worldobj>\n" << std::endl;
}

WorldObj* 
ConveyorBeltData::create_WorldObj ()
{
  return new ConveyorBelt (this);
}

EditorObjLst
ConveyorBeltData::create_EditorObj ()
{
  EditorObjLst lst; 
  lst.push_back(new EditorConveyorBeltObj (*this));
  return lst;
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
ConveyorBelt::draw_offset (int x_of, int y_of, float /*s*/)
{
  left_sur.put_screen (int(pos.x + x_of), int(pos.y + y_of), int(counter));
  for (int i=0; i < width; ++i)
    middle_sur.put_screen (int(pos.x + left_sur.get_width () + i*middle_sur.get_width () + x_of), 
			   int(pos.y + y_of), 
			   int(counter));
  right_sur.put_screen (int(pos.x + left_sur.get_width () + width*middle_sur.get_width () + x_of),
			int(pos.y + y_of), int(counter));
}

void
ConveyorBelt::draw_colmap ()
{
  CL_Surface sur(PingusResource::load_surface("conveyorbelt_cmap", "worldobjs"));
  for (int i=0; i < (width+2); ++i)
    world->get_colmap()->put(sur, int(pos.x) + (15*i), int(pos.y), GroundpieceData::GP_SOLID);
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

/*************************/
/* EditorConveyorBeltObj */
/*************************/

EditorConveyorBeltObj::EditorConveyorBeltObj (const ConveyorBeltData& data)
  : ConveyorBeltData (data)
{
  left_sur   = PingusResource::load_surface ("conveyorbelt_left", "worldobjs");
  right_sur  = PingusResource::load_surface ("conveyorbelt_right", "worldobjs");
  middle_sur = PingusResource::load_surface ("conveyorbelt_middle", "worldobjs");
  
  counter = 0.0f;
}

EditorObj*
EditorConveyorBeltObj::duplicate()
{
  return new EditorConveyorBeltObj (static_cast<ConveyorBeltData> (*this));
}

void
EditorConveyorBeltObj::draw (EditorView * view)
{
  view->draw (left_sur, pos, int(counter));
  for (int i=0; i < ConveyorBeltData::width; ++i)
    view->draw (middle_sur, int(pos.x) + left_sur.get_width () + i*middle_sur.get_width (),
		int(pos.y),
		int(counter));
  view->draw (right_sur,
	      int(pos.x) + left_sur.get_width () + ConveyorBeltData::width*middle_sur.get_width (),
	      int(pos.y), int(counter));
  counter += speed;
  if (counter > 14)
    counter = 0;
  else if (counter < 0)
    counter = middle_sur.get_num_frames () - 1;

}

void
EditorConveyorBeltObj::draw_scroll_map(int /*x_pos*/, int /*y_pos*/, int /*arg_width*/, int /*arg_height*/)
{
  // not supported
}

EditorObjLst
EditorConveyorBeltObj::create (const CL_Vector& pos)
{
  ConveyorBeltData data;
  data.pos = pos;
  return data.create_EditorObj ();
}
  
std::string 
EditorConveyorBeltObj::status_line()
{
  char str[256];
  snprintf (str, 256, "ConveyorBelt - (%f, %f, %f) Speed: %f", pos.x, pos.y, pos.z, speed);
  return str;
}

int 
EditorConveyorBeltObj::get_width ()
{
  return left_sur.get_width() + right_sur.get_width() + width * middle_sur.get_width ();
}

int 
EditorConveyorBeltObj::get_height ()
{
  return middle_sur.get_height ();
}

void 
EditorConveyorBeltObj::set_position_offset(const CL_Vector& offset)
{
  pos += offset;
}

void
EditorConveyorBeltObj::make_larger ()
{
  ConveyorBeltData::width += 1;
}

void
EditorConveyorBeltObj::make_smaller ()
{
  ConveyorBeltData::width -= 1;
  if (ConveyorBeltData::width < 0)
    ConveyorBeltData::width = 0;
}

/* EOF */
