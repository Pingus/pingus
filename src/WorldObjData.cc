//  $Id: WorldObjData.cc,v 1.9 2001/06/11 20:40:16 grumbel Exp $
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

#include "boost/smart_ptr.hpp"
#include "PingusError.hh"
#include "TrapData.hh"
#include "worldobjs/Teleporter.hh"
#include "worldobjs/IceBlock.hh"
#include "worldobjs/ConveyorBelt.hh"
#include "worldobjs/SwitchDoor.hh"
#include "WorldObjData.hh"

boost::shared_ptr<WorldObjData>
WorldObjData::create(xmlDocPtr doc, xmlNodePtr cur)
{
  boost::shared_ptr<WorldObjData> data;
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
      else if (strcmp(type, "conveyorbelt") == 0)
	data = ConveyorBeltData::create (doc, cur);
      else if (strcmp(type, "switchdoor") == 0)
	data = SwitchDoorData::create (doc, cur);
      else
	{
	  throw PingusError("WorldObjData::create (): Unknown type: \"" + std::string(type) + "\"");
	}
    }
  return data;
}

/* EOF */
