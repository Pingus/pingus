//  $Id: worldobj_data_factory.cxx,v 1.14 2002/09/15 21:21:47 grumbel Exp $
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

#include "liquid.hxx"
#include "xml_helper.hxx"
#include "pingus_error.hxx"

// Backgrounds
#include "backgrounds/surface_background_data.hxx"
#include "backgrounds/starfield_background.hxx"
#include "backgrounds/thunderstorm_background_data.hxx"

// Special Objects
#include "hotspot_data.hxx"
#include "entrance_data.hxx"
#include "worldobj_data_factory.hxx"

#include "worldobjsdata/worldobj_group_data.hxx"
#include "exit_data.hxx"
#include "worldobjsdata/bumper_data.hxx"
#include "worldobjsdata/conveyor_belt_data.hxx"
#include "worldobjsdata/fake_exit_data.hxx"
#include "worldobjsdata/guillotine_data.hxx"
#include "worldobjsdata/hammer_data.hxx"
#include "worldobjsdata/ice_block_data.hxx"
#include "worldobjsdata/info_box_data.hxx"
#include "worldobjsdata/laser_exit_data.hxx"
#include "worldobjsdata/smasher_data.hxx"
#include "worldobjsdata/solid_color_background_data.hxx"
#include "worldobjsdata/spike_data.hxx"
#include "worldobjsdata/switch_door_data.hxx"
#include "worldobjsdata/teleporter_data.hxx"

using namespace WorldObjsData;

WorldObjDataFactory* WorldObjDataFactory::instance_ = 0;

WorldObjDataFactory::WorldObjDataFactory ()
{
  // Register all WorldObjData's
}

WorldObjDataFactory* 
WorldObjDataFactory::instance () 
{ 
  if ( ! instance_) 
    {
      instance_ = new WorldObjDataFactory ();
      
      // Registring Factories
      new WorldObjDataFactoryImpl<LiquidData>("liquid");
      new WorldObjDataFactoryImpl<HotspotData>("hotspot");
      new WorldObjDataFactoryImpl<EntranceData>("entrance");
      new WorldObjDataFactoryImpl<ExitData>("exit");
      new WorldObjDataFactoryImpl<WorldObjGroupData>("group");

      // traps
      new WorldObjDataFactoryImpl<BumperData>("bumper");
      new WorldObjDataFactoryImpl<FakeExitData>("fake_exit");
      new WorldObjDataFactoryImpl<GuillotineData>("guillotine");
      new WorldObjDataFactoryImpl<HammerData>("hammer");
      new WorldObjDataFactoryImpl<LaserExitData>("laser_exit");
      new WorldObjDataFactoryImpl<SmasherData>("smasher");
      new WorldObjDataFactoryImpl<SpikeData>("spike");

      // Special Objects 
      new WorldObjDataFactoryImpl<SwitchDoorData>("switchdoor");
      new WorldObjDataFactoryImpl<InfoBoxData>("infobox");
      new WorldObjDataFactoryImpl<IceBlockData>("iceblock");
      new WorldObjDataFactoryImpl<ConveyorBeltData>("conveyorbelt");
      new WorldObjDataFactoryImpl<TeleporterData>("teleporter");

      // Backgrounds
      new WorldObjDataFactoryImpl<SurfaceBackgroundData>("surface-background");
      new WorldObjDataFactoryImpl<StarfieldBackgroundData>("starfield-background");
      new WorldObjDataFactoryImpl<SolidColorBackgroundData>("solidcolor-background");
      new WorldObjDataFactoryImpl<ThunderstormBackgroundData>("thunderstorm-background");
    }

  return instance_;
}

WorldObjData*
WorldObjDataFactory::create (xmlDocPtr doc, xmlNodePtr cur)
{
  std::cout << "WorldObjDataFactory::create (xmlDocPtr doc, xmlNodePtr cur)" << std::endl;
  char* type = XMLhelper::get_prop(cur, "type");
  if (type)
    {
      return create (type, doc, cur);
    }
  else
    {
      // Compatibility stuff
      if (XMLhelper::equal_str(cur->name, "hotspot"))
	{
	  return create ("hotspot", doc, cur);
	}
      else if (XMLhelper::equal_str(cur->name, "exit"))
	{
	  return create ("exit", doc, cur);
	}
      else if (XMLhelper::equal_str(cur->name, "entrance"))
	{
	  return create ("entrance", doc, cur);
	}
      else
	PingusError::raise ("WorldObjDataFactory::create: Error, no type given. - " 
			    + std::string((char*) cur->name));
    }
    
  return 0; //never reached
}

WorldObjData*
WorldObjDataFactory::create (const std::string& id,
			     xmlDocPtr doc, xmlNodePtr cur)
{
  std::cout << "WorldObjDataFactory::create (id, xmlDocPtr doc, xmlNodePtr cur)" << std::endl;

  std::map<std::string, WorldObjDataAbstractFactory*>::iterator it = factories.find(id);
  
  if (it == factories.end())
    PingusError::raise("WorldObjDataFactory: Invalid id: " + id);
  else 
    return it->second->create (doc, cur);
    
  return 0; // never reached
}

void
WorldObjDataFactory::register_factory (const std::string& id,
				       WorldObjDataAbstractFactory* factory)
{
  factories[id] = factory;
}


/* EOF */
