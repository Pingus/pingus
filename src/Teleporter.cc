//  $Id: Teleporter.cc,v 1.1 2000/09/23 20:28:18 grumbel Exp $
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
      if (strcmp((char*)cur->name, "position") == 0)
	{
	  data->pos = XMLhelper::parse_position (doc, cur);
	}
      else if (strcmp((char*)cur->name, "target") == 0)
	{
	  cur = cur->children;
	  if (cur != NULL)
	    data->target_pos = XMLhelper::parse_position (doc, cur);
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
  
}

void 
Teleporter::draw_offset (int x_of, int y_of, float s = 1.0)
{
  
}

void 
Teleporter::let_move(void)
{
  
}

/* EOF */
