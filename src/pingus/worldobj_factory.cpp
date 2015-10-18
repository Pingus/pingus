// Pingus - A free Lemmings clone
// Copyright (C) 2000 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
#include "util/overrride_reader.hpp"
#include "util/mem.hpp"

using namespace WorldObjs;

std::unique_ptr<WorldObjFactory> WorldObjFactory::instance_;

/** WorldObjAbstractFactory, interface for creating factories */
class WorldObjAbstractFactory
{
public:
  WorldObjAbstractFactory() {}
  virtual ~WorldObjAbstractFactory() {}

  virtual std::vector<WorldObj*> create(const ReaderMapping& reader) = 0;

private:
  WorldObjAbstractFactory (const WorldObjAbstractFactory&);
  WorldObjAbstractFactory& operator= (const WorldObjAbstractFactory&);
};

template<class T>
class WorldObjFactoryImpl : public WorldObjAbstractFactory
{
public:
  WorldObjFactoryImpl() {}

  std::vector<WorldObj*> create(const ReaderMapping& reader) {
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

  virtual std::vector<WorldObj*> create(const ReaderMapping& reader)
  {
    std::vector<WorldObj*> group;

    ReaderCollection collection;
    reader.read_collection("objects", collection);
    std::vector<ReaderObject> sections = collection.get_objects();
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

  virtual std::vector<WorldObj*> create(const ReaderMapping& reader)
  {
    std::string name;
    reader.read_string("name", name);

    Vector3f pos;
    reader.read_vector("position", pos);

    PrefabFile prefab = PrefabFile::from_resource(name);
    ReaderMapping overrides;
    reader.read_mapping("overrides", overrides);

    std::vector<WorldObj*> group;
    std::vector<ReaderObject> objects = prefab.get_objects();
    for(auto it = objects.begin(); it != objects.end(); ++it)
    {
      ReaderMapping override_reader = make_override_mapping(it->get_mapping(), overrides);

      std::vector<WorldObj*> objs = WorldObjFactory::instance().create(it->get_name(), override_reader);
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
    instance_->register_factory("group", util::make_unique<WorldObjGroupFactory>());
    instance_->register_factory("prefab", util::make_unique<WorldObjPrefabFactory>());

    instance_->register_factory("liquid", util::make_unique<WorldObjFactoryImpl<Liquid> >());
    instance_->register_factory("hotspot", util::make_unique<WorldObjFactoryImpl<Hotspot> >());
    instance_->register_factory("entrance", util::make_unique<WorldObjFactoryImpl<Entrance> >());
    instance_->register_factory("exit", util::make_unique<WorldObjFactoryImpl<Exit> >());

    // traps
    instance_->register_factory("fake_exit", util::make_unique<WorldObjFactoryImpl<FakeExit> >());
    instance_->register_factory("guillotine", util::make_unique<WorldObjFactoryImpl<Guillotine> >());
    instance_->register_factory("hammer", util::make_unique<WorldObjFactoryImpl<Hammer> >());
    instance_->register_factory("laser_exit", util::make_unique<WorldObjFactoryImpl<LaserExit> >());
    instance_->register_factory("smasher", util::make_unique<WorldObjFactoryImpl<Smasher> >());
    instance_->register_factory("spike", util::make_unique<WorldObjFactoryImpl<Spike> >());

    // Special Objects
    instance_->register_factory("switchdoor-switch", util::make_unique<WorldObjFactoryImpl<SwitchDoorSwitch> >());
    instance_->register_factory("switchdoor-door", util::make_unique<WorldObjFactoryImpl<SwitchDoorDoor> >());
    instance_->register_factory("iceblock", util::make_unique<WorldObjFactoryImpl<IceBlock> >());
    instance_->register_factory("conveyorbelt", util::make_unique<WorldObjFactoryImpl<ConveyorBelt> >());
    instance_->register_factory("teleporter", util::make_unique<WorldObjFactoryImpl<Teleporter> >());
    instance_->register_factory("teleporter-target", util::make_unique<WorldObjFactoryImpl<TeleporterTarget> >());

    // Backgrounds
    instance_->register_factory("surface-background", util::make_unique<WorldObjFactoryImpl<SurfaceBackground> >());
    instance_->register_factory("starfield-background", util::make_unique<WorldObjFactoryImpl<StarfieldBackground> >());
    instance_->register_factory("solidcolor-background", util::make_unique<WorldObjFactoryImpl<SolidColorBackground> >());

    // Weather
    instance_->register_factory("snow-generator", util::make_unique<WorldObjFactoryImpl<SnowGenerator> >());
    instance_->register_factory("rain-generator", util::make_unique<WorldObjFactoryImpl<RainGenerator> >());
    // Weather-Backward compability
    instance_->register_factory("snow", util::make_unique<WorldObjFactoryImpl<SnowGenerator> >());
    instance_->register_factory("rain", util::make_unique<WorldObjFactoryImpl<RainGenerator> >());

    // Groundpieces
    instance_->register_factory("groundpiece", util::make_unique<WorldObjFactoryImpl<Groundpiece> >());
  }

  return *instance_;
}

void WorldObjFactory::deinit()
{
  instance_ = {};
}

std::vector<WorldObj*>
WorldObjFactory::create(const ReaderObject& reader)
{
  return create(reader.get_name(), reader.get_mapping());
}

std::vector<WorldObj*>
WorldObjFactory::create(const std::string& id, const ReaderMapping& reader)
{
  auto it = factories.find(id);

  if (it == factories.end())
  {
    log_error("invalid id: '%1%'", id);
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
