//  $Id: IceBlock.cc,v 1.14 2001/08/04 12:46:23 grumbel Exp $
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
#include "IceBlock.hh"

IceBlockData::IceBlockData ()
{
  width = 1;
}

void
IceBlockData::write_xml(std::ofstream* xml)
{
  (*xml) << "  <worldobj type=\"iceblock\">";
  XMLhelper::write_position_xml (xml, pos);
  (*xml) << "    <width>" << width << "</width>\n"
	 << "  </worldobj>\n" << std::endl;
}

///
boost::shared_ptr<WorldObjData>
IceBlockData::create(xmlDocPtr doc, xmlNodePtr cur)
{
  boost::shared_ptr<IceBlockData> data(new IceBlockData ());

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
      else
	std::cout << "IceBlockData::creata (): Unhandled " << cur->name << std::endl;
      cur = cur->next;
    }
  
  return data;
}



IceBlock::IceBlock (WorldObjData* data)
{
  IceBlockData* ice_block = dynamic_cast<IceBlockData*>(data);
  assert (ice_block);

  pos = ice_block->pos;
  width = ice_block->width;
  block_sur = PingusResource::load_surface ("iceblock", "worldobjs");
  thickness = 1.0;
  is_finished = false;
}

///
IceBlock::~IceBlock ()
{
}

///
void
IceBlock::draw_colmap()
{
  CL_Surface surf (PingusResource::load_surface("iceblock_cmap", "worldobjs"));
  world->get_colmap()->put(surf, (int)pos.x, (int)pos.y, GroundpieceData::GP_GROUND);
}

///
void 
IceBlock::draw_offset(int x_of, int y_of, float s = 1.0)
{
  if (is_finished)
    return;

  block_sur.put_screen (int(pos.x + x_of), int(pos.y + y_of), 
			(int)((1.0 - thickness) * (block_sur.get_num_frames () - 1)));
}

///
void 
IceBlock::update(float delta)
{
  if (is_finished)
    return;

  PinguHolder* holder = world->get_pingu_p();

  for (PinguIter pingu = holder->begin (); pingu != holder->end (); pingu++)
    {
      if ((*pingu)->get_x() > pos.x && (*pingu)->get_x() < pos.x + 25
	  && (*pingu)->get_y() > pos.y - 2 && (*pingu)->get_y() < pos.y + 10)
	{
	  //std::cout << "IceBlock: Catched Pingu: " << thickness  << std::endl;
	  thickness -= 0.02;

	  if (thickness < 0.0)
	    {
	      is_finished = true;
	      thickness = 0.0;
	      CL_Surface surf(PingusResource::load_surface("iceblock_cmap", "worldobjs"));
	      world->get_colmap()->remove(surf, (int)pos.x, (int)pos.y);
	      return;
	    }
	  return;
	}
    }
}

EditorIceBlockObj::EditorIceBlockObj (WorldObjData* obj)
{
  IceBlockData* data = dynamic_cast<IceBlockData*> (obj);
  surf = PingusResource::load_surface ("iceblock", "worldobjs");
  pos = data->pos;
  position = &pos;
  IceBlockData::width = data->width;
}

EditorIceBlockObj::~EditorIceBlockObj ()
{
}

std::list<boost::shared_ptr<EditorObj> > 
EditorIceBlockObj::create (WorldObjData* obj)
{
  std::list<boost::shared_ptr<EditorObj> > objs;
  objs.push_back (boost::shared_ptr<EditorObj>(new EditorIceBlockObj(obj)));
  return objs;
}

/** Create the object with resonable defaults */
std::list<boost::shared_ptr<EditorObj> > 
EditorIceBlockObj::create (const CL_Vector& pos)
{
  IceBlockData data;
  
  data.pos = pos;

  return EditorObj::create (&data);
}

void
EditorIceBlockObj::save_xml (std::ofstream* xml)
{
  this->write_xml (xml);
}

std::string 
EditorIceBlockObj::status_line()
{
  char str[1024];
  sprintf (str, "IceBlock - %f %f %f", pos.x, pos.y, pos.z);
  return str;
}

/* EOF */
