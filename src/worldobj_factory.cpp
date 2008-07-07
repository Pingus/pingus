//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <iostream>
#include "pingus_error.hpp"
#include "worldobj_factory.hpp"

#include "worldobjs/conveyor_belt.hpp"
#include "worldobjs/entrance.hpp"
#include "worldobjs/exit.hpp"
#include "worldobjs/fake_exit.hpp"
#include "worldobjs/groundpiece.hpp"
#include "worldobjs/guillotine.hpp"
#include "worldobjs/hammer.hpp"
#include "worldobjs/hotspot.hpp"
#include "worldobjs/ice_block.hpp"
#include "worldobjs/laser_exit.hpp"
#include "worldobjs/liquid.hpp"
#include "worldobjs/rain_generator.hpp"
#include "worldobjs/smasher.hpp"
#include "worldobjs/snow_generator.hpp"
#include "worldobjs/solid_color_background.hpp"
#include "worldobjs/spike.hpp"
#include "worldobjs/starfield_background.hpp"
#include "worldobjs/surface_background.hpp"
#include "worldobjs/switch_door.hpp"
#include "worldobjs/teleporter.hpp"
#include "worldobjs/teleporter_target.hpp"
#include "worldobjs/thunderstorm_background.hpp"
#include "worldobjs/woodthing.hpp"

using namespace WorldObjs;

WorldObjFactory* WorldObjFactory::instance_ = 0;

/** WorldObjAbstractFactory, interface for creating factories */
class WorldObjAbstractFactory
{
public:
  WorldObjAbstractFactory (const std::string& id) {
    WorldObjFactory::instance ()->register_factory (id, this);
  }

  virtual ~WorldObjAbstractFactory() {}

  virtual WorldObj* create(const FileReader& reader) =0;

private:
  WorldObjAbstractFactory (const WorldObjAbstractFactory&);
  WorldObjAbstractFactory& operator= (const WorldObjAbstractFactory&);
};

/** Template to create factories, usage:
    new WorldObjFactoryImpl<"liquid", Liquid>; */
template<class T>
class WorldObjFactoryImpl : public WorldObjAbstractFactory
{
public:
  WorldObjFactoryImpl (const std::string& id)
    : WorldObjAbstractFactory (id) {}

  WorldObj* create(const FileReader& reader) {
    return new T(reader);
  }

private:
  WorldObjFactoryImpl (const WorldObjFactoryImpl&);
  WorldObjFactoryImpl& operator= (const WorldObjFactoryImpl&);
};

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
			new WorldObjFactoryImpl<WoodThing>("woodthing");
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
      new WorldObjFactoryImpl<IceBlock>("iceblock");
      new WorldObjFactoryImpl<ConveyorBelt>("conveyorbelt");
      new WorldObjFactoryImpl<Teleporter>("teleporter");
      new WorldObjFactoryImpl<TeleporterTarget>("teleporter-target");

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
		instance_ = 0;
	}
}

WorldObj*
WorldObjFactory::create(const FileReader& reader)
{
  std::map<std::string, WorldObjAbstractFactory*>::iterator it = factories.find(reader.get_name());

  if (it == factories.end())
    PingusError::raise("WorldObjFactory: Invalid id: '" + reader.get_name() + "'");
  else
    return it->second->create(reader);

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

/* EOF */
