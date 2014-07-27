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

#include "pingus/worldobj_factory.hpp"

#include <stdexcept>

#include "pingus/prefab_file.hpp"
#include "pingus/worldobjs/conveyor_belt.hpp"
#include "pingus/worldobjs/entrance.hpp"
#include "pingus/worldobjs/exit.hpp"
#include "pingus/worldobjs/fake_exit.hpp"
#include "pingus/worldobjs/groundpiece.hpp"
#include "pingus/worldobjs/guillotine.hpp"
#include "pingus/worldobjs/hammer.hpp"
#include "pingus/worldobjs/hotspot.hpp"
#include "pingus/worldobjs/ice_block.hpp"
#include "pingus/worldobjs/laser_exit.hpp"
#include "pingus/worldobjs/liquid.hpp"
#include "pingus/worldobjs/rain_generator.hpp"
#include "pingus/worldobjs/smasher.hpp"
#include "pingus/worldobjs/snow_generator.hpp"
#include "pingus/worldobjs/solid_color_background.hpp"
#include "pingus/worldobjs/spike.hpp"
#include "pingus/worldobjs/starfield_background.hpp"
#include "pingus/worldobjs/surface_background.hpp"
#include "pingus/worldobjs/switch_door_door.hpp"
#include "pingus/worldobjs/switch_door_switch.hpp"
#include "pingus/worldobjs/teleporter.hpp"
#include "pingus/worldobjs/teleporter_target.hpp"
#include "util/log.hpp"
#include "util/overrride_file_reader.hpp"

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

  virtual std::vector<WorldObj*> create(const FileReader& reader) =0;

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

  std::vector<WorldObj*> create(const FileReader& reader) {
    std::vector<WorldObj*> lst;
    lst.push_back(new T(reader));
    return lst;
  }

private:
  WorldObjFactoryImpl (const WorldObjFactoryImpl&);
  WorldObjFactoryImpl& operator= (const WorldObjFactoryImpl&);
};

class WorldObjGroupFactory : public WorldObjAbstractFactory
{
public:
  WorldObjGroupFactory (const std::string& id) :
    WorldObjAbstractFactory(id)
  {}

  virtual ~WorldObjGroupFactory() {}

  virtual std::vector<WorldObj*> create(const FileReader& reader) {
    std::vector<WorldObj*> group;
    
    FileReader objects = reader.read_section("objects");
    std::vector<FileReader> sections = objects.get_sections();
    for(auto it = sections.begin(); it != sections.end(); ++it)
    {
      std::vector<WorldObj*> objs = WorldObjFactory::instance()->create(*it);
      for(auto obj = objs.begin(); obj != objs.end(); ++obj)
      {
        if (*obj)
        {
          group.push_back(*obj);
        }
      }
    }
    return group;
  }

private:
  WorldObjGroupFactory (const WorldObjGroupFactory&);
  WorldObjGroupFactory& operator= (const WorldObjGroupFactory&);
};

class WorldObjPrefabFactory : public WorldObjAbstractFactory
{
public:
  WorldObjPrefabFactory (const std::string& id) :
    WorldObjAbstractFactory(id)
  {}

  virtual ~WorldObjPrefabFactory() {}

  virtual std::vector<WorldObj*> create(const FileReader& reader) {
    std::string name;
    reader.read_string("name", name);

    Vector3f pos;
    reader.read_vector("position", pos);

    PrefabFile prefab = PrefabFile::from_resource(name);
    FileReader overrides;
    reader.read_section("overrides", overrides);

    std::vector<WorldObj*> group;
    const std::vector<FileReader>& objects = prefab.get_objects();
    for(auto it = objects.begin(); it != objects.end(); ++it)
    {
      OverrideFileReader override_reader(*it, overrides);

      std::vector<WorldObj*> objs = WorldObjFactory::instance()->create(override_reader);
      for(auto obj = objs.begin(); obj != objs.end(); ++obj)
      {
        if (*obj)
        {
          (*obj)->set_pos((*obj)->get_pos() + pos);
          group.push_back(*obj);
        }
      }
    }
    return group;
  }

private:
  WorldObjPrefabFactory (const WorldObjPrefabFactory&);
  WorldObjPrefabFactory& operator= (const WorldObjPrefabFactory&);
};

WorldObjFactory::WorldObjFactory() :
  factories()
{
  // Register all WorldObj's
}

WorldObjFactory*
WorldObjFactory::instance()
{
  if ( ! instance_)
  {
    instance_ = new WorldObjFactory ();

    // Registring Factories
    new WorldObjGroupFactory("group");
    new WorldObjPrefabFactory("prefab");

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
    new WorldObjFactoryImpl<SwitchDoorSwitch>("switchdoor-switch");
    new WorldObjFactoryImpl<SwitchDoorDoor>("switchdoor-door");
    new WorldObjFactoryImpl<IceBlock>("iceblock");
    new WorldObjFactoryImpl<ConveyorBelt>("conveyorbelt");
    new WorldObjFactoryImpl<Teleporter>("teleporter");
    new WorldObjFactoryImpl<TeleporterTarget>("teleporter-target");

    // Backgrounds
    new WorldObjFactoryImpl<SurfaceBackground>("surface-background");
    new WorldObjFactoryImpl<StarfieldBackground>("starfield-background");
    new WorldObjFactoryImpl<SolidColorBackground>("solidcolor-background");

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

std::vector<WorldObj*>
WorldObjFactory::create(const FileReader& reader)
{
  std::map<std::string, WorldObjAbstractFactory*>::iterator it = factories.find(reader.get_name());

  if (it == factories.end())
  {
    log_error("invalid id: '%1%'", reader.get_name());
    return std::vector<WorldObj*>();
  }
  else
  {
    return it->second->create(reader);
  }
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
