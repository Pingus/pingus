//  $Id: WorldObjDataFactory.cc,v 1.2 2001/08/10 10:56:13 grumbel Exp $
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

#include "Liquid.hh"
#include "worldobjs/SwitchDoor.hh"
#include "worldobjs/IceBlock.hh"
#include "worldobjs/ConveyorBelt.hh"
#include "worldobjs/Teleporter.hh"

#include "WorldObjDataFactory.hh"

WorldObjDataFactory* WorldObjDataFactory::instance_ = 0;

WorldObjDataFactory::WorldObjDataFactory ()
{
  // Register all WorldObjData's
}

WorldObjDataFactory* 
WorldObjDataFactory::instance () 
{ 
  if (instance_ == 0) 
    {
      instance_ = new WorldObjDataFactory ();
      
      // Registring Factories
      new WorldObjDataFactoryImpl<TrapData>("trap");
      new WorldObjDataFactoryImpl<LiquidData>("liquid");
      new WorldObjDataFactoryImpl<SwitchDoorData>("switchdoor");
      new WorldObjDataFactoryImpl<IceBlockData>("iceblock");
      new WorldObjDataFactoryImpl<ConveyorBeltData>("conveyorbelt");
      new WorldObjDataFactoryImpl<TeleporterData>("teleporter");
      new WorldObjDataFactoryImpl<HotspotData>("hotspot");
      new WorldObjDataFactoryImpl<EntranceData>("hotspot");
    }

  return instance_;
}

boost::shared_ptr<WorldObjData>
WorldObjDataFactory::create (xmlDocPtr doc, xmlNodePtr cur)
{
  std::cout << "WorldObjDataFactory::create (xmlDocPtr doc, xmlNodePtr cur)" << std::endl;
  char* type = (char*)xmlGetProp(cur, (xmlChar*)"type");
  if (type)
    {
      std::string id (type);
      free (type);
      
      std::map<std::string, WorldObjDataAbstractFactory*>::iterator it = factories.find(id);
      
      if (it == factories.end())
	throw PingusError("WorldObjDataFactory: Invalid id: " + id);
      else 
	return boost::shared_ptr<WorldObjData> (it->second->create (doc, cur));
    }
  else
    {
      throw PingusError ("WorldObjDataFactory::create: Error, no type given.");
    }
}

void
WorldObjDataFactory::register_factory (const std::string& id,
				       WorldObjDataAbstractFactory* factory)
{
  factories[id] = factory;
}


/* EOF */
