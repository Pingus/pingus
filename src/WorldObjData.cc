//  $Id: WorldObjData.cc,v 1.5 2000/11/14 22:22:56 grumbel Exp $
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

#include "TrapData.hh"
#include "worldobjs/Teleporter.hh"
#include "worldobjs/IceBlock.hh"
#include "WorldObjData.hh"

WorldObjData* 
WorldObjData::create(xmlDocPtr doc, xmlNodePtr cur)
{
  std::cout << "WroldObjDATa:create" << std::endl;
  WorldObjData* data = 0;
  char* type = (char*)xmlGetProp(cur, (xmlChar*)"type");
  
  if (type == 0)
    {
      std::cout << "WorldObjData::create (): No type given" << std::endl;
    }
  else
    {
      if (strcmp(type, "teleporter") == 0)
	data = TeleporterData::create (doc, cur);
      else if (strcmp(type, "iceblock") == 0)
	data = IceBlockData::create (doc, cur);
      else if (strcmp(type, "trap") == 0)	
	data = TrapData::create (doc, cur);
      else
	{
	  std::cout << "WorldObjData::create (): Unknown type: \"" << type << "\"" << std::endl;
	}
    }
  return data;
}

/* EOF */
