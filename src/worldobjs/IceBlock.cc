//  $Id: IceBlock.cc,v 1.1 2000/11/14 22:22:56 grumbel Exp $
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
  (*xml) << "    <width>" << width << "</width>"
	 << "  </worldobj>" << std::endl;
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
}

///
IceBlock::~IceBlock ()
{
}

///
void
IceBlock::draw_colmap()
{
  CL_SurfaceProvider* prov
    = PingusResource::load_surface("worldobjs/iceblock_cmap", "traps")->get_provider();
  world->get_colmap()->put(prov, pos.x_pos, pos.y_pos, GroundpieceData::SOLID);
}

///
void 
IceBlock::draw_offset(int x_of, int y_of, float s = 1.0)
{
  if (thickness <= 0.0)
    return;

  //std::cout << "thick: " << thickness << " " << (1.0 - thickness) * (block_sur->get_num_frames () - 1) << std::endl;
  block_sur->put_screen (pos.x_pos + x_of, pos.y_pos + y_of, 
			 (int)((1.0 - thickness) * (block_sur->get_num_frames () - 1)));
}

///
void 
IceBlock::let_move()
{

  if (thickness == 0.0)
    {
      return ;
    }


  PinguHolder* holder = world->get_pingu_p();

  for (PinguIter pingu = holder->begin (); pingu != holder->end (); pingu++)
    {
      if ((*pingu)->get_x() > pos.x_pos && (*pingu)->get_x() < pos.x_pos + 25
	  && (*pingu)->get_y() > pos.y_pos - 2 && (*pingu)->get_y() < pos.y_pos + 10)
	{
	  //std::cout << "IceBlock: Catched Pingu: " << thickness  << std::endl;
	  thickness -= 0.1;
	  return;
	}
    }

  if (thickness <= 0.0)
    {
      thickness = 0.0;
      CL_SurfaceProvider* prov
	= PingusResource::load_surface("worldobjs/iceblock_cmap", "traps")->get_provider();
      world->get_colmap()->remove(prov, pos.x_pos, pos.y_pos);
      return;
    }
}

EditorIceBlockObj::EditorIceBlockObj ()
{
}

EditorIceBlockObj::EditorIceBlockObj (WorldObjData* obj)
{
  IceBlockData* data = dynamic_cast<IceBlockData*> (obj);
  surf = PingusResource::load_surface ("iceblock", "worldobjs");
  EditorObj::pos = data->pos;
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

void
EditorIceBlockObj::save_xml (std::ofstream* xml)
{
  IceBlockData::pos = EditorObj::pos;
  this->write_xml (xml);
}

std::string 
EditorIceBlockObj::status_line()
{
  return "IceBlock";
}

/* EOF */
