//  $Id: IceBlock.cc,v 1.7 2000/12/14 21:35:56 grumbel Exp $
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
#include "IceBlock.hh"

IceBlockData::IceBlockData ()
{
  width = 1;
}

void
IceBlockData::write_xml(ofstream* xml)
{
  (*xml) << "  <worldobj type=\"iceblock\">";
  XMLhelper::write_position_xml (xml, pos);
  (*xml) << "    <width>" << width << "</width>\n"
	 << "  </worldobj>\n" << std::endl;
}

///
WorldObjData* 
IceBlockData::create(xmlDocPtr doc, xmlNodePtr cur)
{
  IceBlockData* data = new IceBlockData ();

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
  world->get_colmap()->put(surf, pos.x_pos, pos.y_pos, GroundpieceData::GROUND);
}

///
void 
IceBlock::draw_offset(int x_of, int y_of, float s = 1.0)
{
  if (is_finished)
    return;

  block_sur.put_screen (pos.x_pos + x_of, pos.y_pos + y_of, 
			(int)((1.0 - thickness) * (block_sur.get_num_frames () - 1)));
}

///
void 
IceBlock::let_move()
{
  if (is_finished)
    return;

  PinguHolder* holder = world->get_pingu_p();

  for (PinguIter pingu = holder->begin (); pingu != holder->end (); pingu++)
    {
      if ((*pingu)->get_x() > pos.x_pos && (*pingu)->get_x() < pos.x_pos + 25
	  && (*pingu)->get_y() > pos.y_pos - 2 && (*pingu)->get_y() < pos.y_pos + 10)
	{
	  //std::cout << "IceBlock: Catched Pingu: " << thickness  << std::endl;
	  thickness -= 0.02;

	  if (thickness < 0.0)
	    {
	      is_finished = true;
	      thickness = 0.0;
	      CL_Surface surf(PingusResource::load_surface("iceblock_cmap", "worldobjs"));
	      world->get_colmap()->remove(surf, pos.x_pos, pos.y_pos);
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

std::list<EditorObj*> 
EditorIceBlockObj::create (WorldObjData* obj)
{
  std::list<EditorObj*> objs;
  objs.push_back (new EditorIceBlockObj(obj));
  return objs;
}

/** Create the object with resonable defaults */
std::list<EditorObj*> 
EditorIceBlockObj::create (const Position& pos)
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
  sprintf (str, "IceBlock - %d %d %d", pos.x_pos, pos.y_pos, pos.z_pos);
  return str;
}

/* EOF */
