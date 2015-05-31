//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmail.com>
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

std::unique_ptr<WorldObjFactory> WorldObjFactory::instance_;

/** WorldObjAbstractFactory, interface for creating factories */
class WorldObjAbstractFactory
{
public:
  WorldObjAbstractFactory() {}
  virtual ~WorldObjAbstractFactory() {}

  virtual std::vector<WorldObj*> create(const FileReader& reader) =0;

private:
  WorldObjAbstractFactory (const WorldObjAbstractFactory&);
  WorldObjAbstractFactory& operator= (const WorldObjAbstractFactory&);
};

template<class T>
class WorldObjFactoryImpl : public WorldObjAbstractFactory
{
public:
  WorldObjFactoryImpl() {}

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
  WorldObjGroupFactory() {}
  virtual ~WorldObjGroupFactory() {}

  virtual std::vector<WorldObj*> create(const FileReader& reader) {
    std::vector<WorldObj*> group;

    FileReader objects = reader.read_section("objects");
    std::vector<FileReader> sections = objects.get_sections();
    for(auto it = sections.begin(); it != sections.end(); ++it)
    {
      std::vector<WorldObj*> objs = WorldObjFactory::instance().create(*it);
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
  WorldObjPrefabFactory() {}
  virtual ~WorldObjPrefabFactory() {}

  virtual std::vector<WorldObj*> create(const FileReader& reader)
  {
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

      std::vector<WorldObj*> objs = WorldObjFactory::instance().create(override_reader);
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

WorldObjFactory&
WorldObjFactory::instance()
{
  if (!instance_)
  {
    instance_ = std::unique_ptr<WorldObjFactory>(new WorldObjFactory);

    // Registring Factories
    instance_->register_factory("group", std::make_unique<WorldObjGroupFactory>());
    instance_->register_factory("prefab", std::make_unique<WorldObjPrefabFactory>());

    instance_->register_factory("liquid", std::make_unique<WorldObjFactoryImpl<Liquid> >());
    instance_->register_factory("hotspot", std::make_unique<WorldObjFactoryImpl<Hotspot> >());
    instance_->register_factory("entrance", std::make_unique<WorldObjFactoryImpl<Entrance> >());
    instance_->register_factory("exit", std::make_unique<WorldObjFactoryImpl<Exit> >());

    // traps
    instance_->register_factory("fake_exit", std::make_unique<WorldObjFactoryImpl<FakeExit> >());
    instance_->register_factory("guillotine", std::make_unique<WorldObjFactoryImpl<Guillotine> >());
    instance_->register_factory("hammer", std::make_unique<WorldObjFactoryImpl<Hammer> >());
    instance_->register_factory("laser_exit", std::make_unique<WorldObjFactoryImpl<LaserExit> >());
    instance_->register_factory("smasher", std::make_unique<WorldObjFactoryImpl<Smasher> >());
    instance_->register_factory("spike", std::make_unique<WorldObjFactoryImpl<Spike> >());

    // Special Objects
    instance_->register_factory("switchdoor-switch", std::make_unique<WorldObjFactoryImpl<SwitchDoorSwitch> >());
    instance_->register_factory("switchdoor-door", std::make_unique<WorldObjFactoryImpl<SwitchDoorDoor> >());
    instance_->register_factory("iceblock", std::make_unique<WorldObjFactoryImpl<IceBlock> >());
    instance_->register_factory("conveyorbelt", std::make_unique<WorldObjFactoryImpl<ConveyorBelt> >());
    instance_->register_factory("teleporter", std::make_unique<WorldObjFactoryImpl<Teleporter> >());
    instance_->register_factory("teleporter-target", std::make_unique<WorldObjFactoryImpl<TeleporterTarget> >());

    // Backgrounds
    instance_->register_factory("surface-background", std::make_unique<WorldObjFactoryImpl<SurfaceBackground> >());
    instance_->register_factory("starfield-background", std::make_unique<WorldObjFactoryImpl<StarfieldBackground> >());
    instance_->register_factory("solidcolor-background", std::make_unique<WorldObjFactoryImpl<SolidColorBackground> >());

    // Weather
    instance_->register_factory("snow-generator", std::make_unique<WorldObjFactoryImpl<SnowGenerator> >());
    instance_->register_factory("rain-generator", std::make_unique<WorldObjFactoryImpl<RainGenerator> >());
    // Weather-Backward compability
    instance_->register_factory("snow", std::make_unique<WorldObjFactoryImpl<SnowGenerator> >());
    instance_->register_factory("rain", std::make_unique<WorldObjFactoryImpl<RainGenerator> >());

    // Groundpieces
    instance_->register_factory("groundpiece", std::make_unique<WorldObjFactoryImpl<Groundpiece> >());
  }

  return *instance_;
}

void WorldObjFactory::deinit()
{
  instance_ = {};
}

std::vector<WorldObj*>
WorldObjFactory::create(const FileReader& reader)
{
  auto it = factories.find(reader.get_name());

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
WorldObjFactory::register_factory(const std::string& id,
                                  std::unique_ptr<WorldObjAbstractFactory> factory)
{
  factories[id] = std::move(factory);
}

/* EOF */
