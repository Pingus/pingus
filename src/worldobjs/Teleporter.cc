//  $Id: Teleporter.cc,v 1.11 2000/12/16 23:11:24 grumbel Exp $
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
#include "../PingusResource.hh"
#include "../PinguHolder.hh"
#include "../XMLhelper.hh"
#include "Teleporter.hh"

TeleporterData::TeleporterData (const TeleporterData& data)
{
  pos = data.pos;
  target_pos = data.target_pos;
}

void 
TeleporterData::write_xml(ofstream* xml)
{
  (*xml) << "  <worldobj type=\"teleporter\">";
  XMLhelper::write_position_xml (xml, pos);
  (*xml) << "    <target>" << std::endl;
  XMLhelper::write_position_xml (xml, pos);
  (*xml) << "    </target>" << std::endl;
  (*xml) << "  </worldobj>" << std::endl;
}

boost::shared_ptr<WorldObjData>
TeleporterData::create(xmlDocPtr doc, xmlNodePtr cur)
{
  boost::shared_ptr<TeleporterData> data(new TeleporterData ());
  
  cur = cur->children;
  
  while (cur != NULL)
    {
      if (xmlIsBlankNode(cur)) 
	{
	  cur = cur->next;
	  continue;
	}

      if (strcmp((char*)cur->name, "position") == 0)
	{
	  data->pos = XMLhelper::parse_position (doc, cur);
	}
      else if (strcmp((char*)cur->name, "target") == 0)
	{
	  xmlNodePtr ncur = cur->children;

	  if (xmlIsBlankNode(ncur)) ncur = ncur->next;
	    
	  if (ncur != NULL)
	    data->target_pos = XMLhelper::parse_position (doc, ncur);
	  else
	    std::cout << "TeleporterData::create (): <target> is empty" << std::endl;
	}
      else
	{
	  std::cout << "TeleportData::creata (): Unhandled " << cur->name << std::endl;
	}

      cur = cur->next;
    }
  return data;
}


/**************/
/* Teleporter */
/**************/

Teleporter::Teleporter (boost::shared_ptr<WorldObjData> data)
{
  TeleporterData* teleporter = dynamic_cast<TeleporterData*>(data.get());
  assert (teleporter);
  
  sur = PingusResource::load_surface("teleporter", "worldobjs");

  pos = teleporter->pos;
  target_pos = teleporter->target_pos;

  std::cout << "pos: " << pos.x_pos << " "  << pos.y_pos << " " << pos.z_pos << std::endl;
}

void 
Teleporter::draw_offset (int x_of, int y_of, float s = 1.0)
{
  //std::cout << "Teleporter::draw_offset ()" << std::endl;
  sur.put_screen (pos.x_pos + x_of, pos.y_pos + y_of);
}

void 
Teleporter::let_move ()
{
  PinguHolder* holder = world->get_pingu_p();

  for (PinguIter pingu = holder->begin (); pingu != holder->end (); pingu++)
    {
      if ((*pingu)->get_x() > pos.x_pos  && (*pingu)->get_x() < pos.x_pos + 35
	  && (*pingu)->get_y() > pos.y_pos && (*pingu)->get_y() < pos.y_pos + 52)
	{
	  (*pingu)->set_pos (target_pos.x_pos, target_pos.y_pos);
	}
    }
}

EditorTeleporterObj::EditorTeleporterObj (WorldObjData* obj)
{
  surf = PingusResource::load_surface ("teleporter", "worldobjs");
  width = surf.get_width ();
  height = surf.get_height ();

  TeleporterData* data = dynamic_cast<TeleporterData*> (obj);  
  assert (data);
  
  pos = data->pos;
  position = &pos;
  target_pos = data->target_pos;
  std::cout << "EditorTeleporter(): " << &target_pos << " - " << target_pos << std::endl;
}

EditorTeleporterObj::~EditorTeleporterObj ()
{
  // FIXME: delete the target obj here
}

std::list<shared_ptr<EditorObj> > 
EditorTeleporterObj::create (WorldObjData* data)
{
  std::list<shared_ptr<EditorObj> > objs;

  TeleporterData* tdata = dynamic_cast<TeleporterData*> (data);    
  std::cout << "EditorTeleporterObj: " << tdata << " - " << tdata->target_pos << std::endl;

  shared_ptr<EditorTeleporterObj> teleporter(new EditorTeleporterObj (data));
  shared_ptr<EditorTeleporterTargetObj> teleporter_target(new EditorTeleporterTargetObj (data, teleporter->get_target_pos_p ()));

  objs.push_back (teleporter);
  objs.push_back (teleporter_target);

  return objs;
}

std::list<boost::shared_ptr<EditorObj> >
EditorTeleporterObj::create (const Position& pos)
{
  TeleporterData data;

  std::cout << "EditorTeleporterObj: creating..." << std::endl;

  data.pos = pos;
  data.target_pos.x_pos = pos.x_pos + 50;
  data.target_pos.y_pos = pos.y_pos + 50;

  return EditorObj::create (&data);
}

void
EditorTeleporterObj::save_xml (std::ofstream* xml)
{
  // Before we write down the xml stuff, we need to get the positions
  // of the objects
  //  TeleporterData::target_pos = target->get_position ();
  this->write_xml (xml);
}

std::string
EditorTeleporterObj::status_line()
{
  // FIXME: replace with string streams
  char str[1024];
  sprintf (str, "Teleporter - %d %d %d", 
	   pos.x_pos, pos.y_pos, pos.z_pos);
  return str;
}

EditorTeleporterTargetObj::EditorTeleporterTargetObj ()
{
}

EditorTeleporterTargetObj::EditorTeleporterTargetObj (WorldObjData* obj, Position* pos)
{
  surf = PingusResource::load_surface ("teleporter2", "worldobjs");
  width = surf.get_width ();
  height = surf.get_height ();

  TeleporterData* data = dynamic_cast<TeleporterData*> (obj);
  assert (data);
  assert (pos);
  //*pos = data->target_pos;
  position = pos;

  std::cout << "EditorTeleporterTargetObj: " << pos << " - " << *pos << std::endl;
}

void
EditorTeleporterObj::draw_offset(int x_of, int y_of)
{
  CL_Display::draw_line (pos.x_pos + x_of + width/2, pos.y_pos + y_of + height/2,
			 target_pos.x_pos + x_of + 32, target_pos.y_pos + y_of + 32,
			 0.0, 1.0, 0.0, 0.5);
  EditorObj::draw_offset (x_of, y_of);
}

void
EditorTeleporterTargetObj::save_xml (std::ofstream* xml)
{
  // do nothing, all the saving is done inside the EditorTeleporterObj
}

std::string
EditorTeleporterTargetObj::status_line()
{
  char str[1024];
  sprintf (str, "TeleporterTarget - %d %d %d", 
	   position->x_pos, position->y_pos, position->z_pos);
  return str;
}

/* EOF */
