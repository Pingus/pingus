//  $Id: Teleporter.cc,v 1.2 2000/09/24 00:22:06 grumbel Exp $
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

#include "PingusResource.hh"
#include "PinguHolder.hh"
#include "XMLhelper.hh"
#include "Teleporter.hh"

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

WorldObjData* 
TeleporterData::create(xmlDocPtr doc, xmlNodePtr cur)
{
  TeleporterData* data = new TeleporterData ();
  
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

Teleporter::Teleporter (WorldObjData* data)
{
  TeleporterData* teleporter = dynamic_cast<TeleporterData*>(data);
  assert (teleporter);
  
  sur = PingusResource::load_surface("Traps/teleporter", "traps");

  pos = teleporter->pos;
  target_pos = teleporter->target_pos;

  std::cout << "pos: " << pos.x_pos << " "  << pos.y_pos << " " << pos.z_pos << std::endl;
}

void 
Teleporter::draw_offset (int x_of, int y_of, float s = 1.0)
{
  std::cout << "Teleporter::draw_offset ()" << std::endl;
  sur->put_screen (pos.x_pos + x_of, pos.y_pos + y_of);
}

void 
Teleporter::let_move ()
{
  std::cout << "Teleporter::let_move ()" << std::endl;  

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

/* EOF */
