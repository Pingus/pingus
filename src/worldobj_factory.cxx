//  $Id: worldobj_data_factory.cxx,v 1.36 2003/10/18 23:17:27 grumbel Exp $
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

#include <iostream>
#include <ClanLib/Core/System/clanstring.h>
#include "pingus_error.hxx"
#include "worldobj_factory.hxx"
#include "xml_helper.hxx"

#include "worldobjs/conveyor_belt.hxx"
#include "worldobjs/entrance.hxx"
#include "worldobjs/exit.hxx"
#include "worldobjs/fake_exit.hxx"
#include "worldobjs/groundpiece.hxx"
#include "worldobjs/guillotine.hxx"
#include "worldobjs/hammer.hxx"
#include "worldobjs/hotspot.hxx"
#include "worldobjs/ice_block.hxx"
#include "worldobjs/info_box.hxx"
#include "worldobjs/laser_exit.hxx"
#include "worldobjs/liquid.hxx"
#include "worldobjs/rain_generator.hxx"
#include "worldobjs/smasher.hxx"
#include "worldobjs/snow_generator.hxx"
#include "worldobjs/solid_color_background.hxx"
#include "worldobjs/spike.hxx"
#include "worldobjs/starfield_background.hxx"
#include "worldobjs/surface_background.hxx"
#include "worldobjs/switch_door.hxx"
#include "worldobjs/teleporter.hxx"
#include "worldobjs/thunderstorm_background.hxx"

namespace Pingus {

using namespace WorldObjs;

WorldObjFactory* WorldObjFactory::instance_ = 0;

WorldObjFactory::WorldObjFactory ()
{
  // Register all WorldObj's
}

WorldObjFactory*
WorldObjFactory::instance ()
{
  if ( ! instance_)
    {
      instance_ = new WorldObjFactory ();

      // Registring Factories
      new WorldObjFactoryImpl<Liquid>("liquid");
      new WorldObjFactoryImpl<Hotspot>("hotspot");
      new WorldObjFactoryImpl<Entrance>("entrance");
      new WorldObjFactoryImpl<Exit>("exit");

      // traps
      new WorldObjFactoryImpl<FakeExit>("fake_exit");
      new WorldObjFactoryImpl<Guillotine>("guillotine");
      new WorldObjFactoryImpl<Hammer>("hammer");
      new WorldObjFactoryImpl<LaserExit>("laser_exit");
      new WorldObjFactoryImpl<Smasher>("smasher");
      new WorldObjFactoryImpl<Spike>("spike");

      // Special Objects
      new WorldObjFactoryImpl<SwitchDoor>("switchdoor");
      new WorldObjFactoryImpl<InfoBox>("infobox");
      new WorldObjFactoryImpl<IceBlock>("iceblock");
      new WorldObjFactoryImpl<ConveyorBelt>("conveyorbelt");
      new WorldObjFactoryImpl<Teleporter>("teleporter");

      // Backgrounds
      new WorldObjFactoryImpl<SurfaceBackground>("surface-background");
      new WorldObjFactoryImpl<StarfieldBackground>("starfield-background");
      new WorldObjFactoryImpl<SolidColorBackground>("solidcolor-background");
      new WorldObjFactoryImpl<ThunderstormBackground>("thunderstorm-background");

      // Weather
      new WorldObjFactoryImpl<SnowGenerator>("snow-generator");
      new WorldObjFactoryImpl<RainGenerator>("rain-generator");
      // Weather-Backward compability
      new WorldObjFactoryImpl<SnowGenerator>("snow");
      new WorldObjFactoryImpl<RainGenerator>("rain");

      // Groundpieces
      new WorldObjFactoryImpl<Groundpiece>("groundpiece");
    }

  return instance_;
}

void WorldObjFactory::deinit()
{
	if (instance_)
	{
		instance_->free_factories();
		delete instance_;
	}
}

WorldObj*
WorldObjFactory::create (xmlDocPtr doc, xmlNodePtr cur)
{
  //std::cout << "WorldObjFactory::create (xmlDocPtr doc, xmlNodePtr cur)" << std::endl;

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
  else if (XMLhelper::equal_str(cur->name, "groundpiece"))
    {
      return create ("groundpiece", doc, cur);
    }
  else if (XMLhelper::equal_str(cur->name, "prefab"))
    {
      return create ("prefab", doc, cur);
    }
  else if (XMLhelper::equal_str(cur->name, "liquid"))
    {
      return create ("liquid", doc, cur);
    }
  else
    {
      std::string type;
      if (XMLhelper::get_prop(cur, "type", type))
        {
          return create (type, doc, cur);
	}
      else
	{
	  PingusError::raise ("WorldObjFactory::create: Error, no type given. - "
			      + std::string((char*) cur->name));
	}
    }

  return 0; //never reached
}

WorldObj*
WorldObjFactory::create (const std::string& id,
			     xmlDocPtr doc, xmlNodePtr cur)
{
  //std::cout << "WorldObjFactory::create (id, xmlDocPtr doc, xmlNodePtr cur)" << std::endl;

  std::map<std::string, WorldObjAbstractFactory*>::iterator it = factories.find(id);

  if (it == factories.end())
    PingusError::raise("WorldObjFactory: Invalid id: '" + id + "' at line "
		       + CL_String::to(XMLhelper::get_line(cur)));
  else
    return it->second->create (doc, cur);

  return 0; // never reached
}

void
WorldObjFactory::register_factory (const std::string& id,
				       WorldObjAbstractFactory* factory)
{
  factories[id] = factory;
}

void
WorldObjFactory::free_factories()
{
  for (std::map<std::string, WorldObjAbstractFactory*>::iterator i = factories.begin(); i != factories.end(); ++i)
  {
	  delete i->second;
  }
}

} // namespace Pingus

/* EOF */
