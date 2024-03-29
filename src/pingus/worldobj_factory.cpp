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

#include <logmich/log.hpp>

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
#include "util/reader.hpp"

using namespace pingus::worldobjs;

namespace pingus {

std::unique_ptr<WorldObjFactory> WorldObjFactory::instance_;

/** WorldObjAbstractFactory, interface for creating factories */
class WorldObjAbstractFactory
{
public:
  WorldObjAbstractFactory() {}
  virtual ~WorldObjAbstractFactory() {}

  virtual std::vector<WorldObj*> create(ReaderMapping const& reader) = 0;

private:
  WorldObjAbstractFactory (WorldObjAbstractFactory const&);
  WorldObjAbstractFactory& operator= (WorldObjAbstractFactory const&);
};

template<class T>
class WorldObjFactoryImpl : public WorldObjAbstractFactory
{
public:
  WorldObjFactoryImpl() {}

  std::vector<WorldObj*> create(ReaderMapping const& reader) override {
    std::vector<WorldObj*> lst;
    lst.push_back(new T(reader));
    return lst;
  }

private:
  WorldObjFactoryImpl (WorldObjFactoryImpl const&);
  WorldObjFactoryImpl& operator= (WorldObjFactoryImpl const&);
};

class WorldObjGroupFactory : public WorldObjAbstractFactory
{
public:
  WorldObjGroupFactory() {}
  ~WorldObjGroupFactory() override {}

  std::vector<WorldObj*> create(ReaderMapping const& reader) override
  {
    std::vector<WorldObj*> group;

    ReaderCollection collection;
    reader.read("objects", collection);
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
  WorldObjGroupFactory (WorldObjGroupFactory const&);
  WorldObjGroupFactory& operator= (WorldObjGroupFactory const&);
};

class WorldObjPrefabFactory : public WorldObjAbstractFactory
{
public:
  WorldObjPrefabFactory() {}
  ~WorldObjPrefabFactory() override {}

  std::vector<WorldObj*> create(ReaderMapping const& reader) override
  {
    std::string name;
    reader.read("name", name);

    Vector2f pos;
    float z_index = 0.0f;
    InVector2fZ in_vec{pos, z_index};
    reader.read("position", in_vec);

    PrefabFile prefab = PrefabFile::from_resource(name);
    ReaderMapping overrides;
    reader.read("overrides", overrides);

    std::vector<WorldObj*> group;
    auto const& objects = prefab.get_objects().get_objects();
    for(auto it = objects.begin(); it != objects.end(); ++it)
    {
      ReaderMapping it_mapping = it->get_mapping();
      ReaderMapping override_reader = make_override_mapping(it_mapping, overrides);

      std::vector<WorldObj*> objs = WorldObjFactory::instance().create(it->get_name(), override_reader);
      for(auto obj = objs.begin(); obj != objs.end(); ++obj)
      {
        if (*obj)
        {
          (*obj)->set_pos((*obj)->get_pos() + geom::foffset(pos.x(), pos.y()));
          (*obj)->set_z_index((*obj)->z_index() + z_index);
          group.push_back(*obj);
        }
      }
    }
    return group;
  }

private:
  WorldObjPrefabFactory (WorldObjPrefabFactory const&);
  WorldObjPrefabFactory& operator= (WorldObjPrefabFactory const&);
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
WorldObjFactory::create(ReaderObject const& reader)
{
  return create(reader.get_name(), reader.get_mapping());
}

std::vector<WorldObj*>
WorldObjFactory::create(std::string const& id, ReaderMapping const& reader)
{
  auto it = factories.find(id);

  if (it == factories.end())
  {
    log_error("invalid id: '{}'", id);
    return std::vector<WorldObj*>();
  }
  else
  {
    return it->second->create(reader);
  }
}

void
WorldObjFactory::register_factory(std::string const& id,
                                  std::unique_ptr<WorldObjAbstractFactory> factory)
{
  factories[id] = std::move(factory);
}

} // namespace pingus

/* EOF */
