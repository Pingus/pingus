// Pingus - A free Lemmings clone
// Copyright (C) 1999 Ingo Ruhnke <grumbel@gmail.com>
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

#include "pingus/world.hpp"

#include <algorithm>
#include <functional>
#include <optional>

#include <logmich/log.hpp>

#include "engine/display/scene_context.hpp"
#include "engine/sound/sound.hpp"
#include "pingus/collision_map.hpp"
#include "pingus/ground_map.hpp"
#include "pingus/particles/pingu_particle_holder.hpp"
#include "pingus/particles/rain_particle_holder.hpp"
#include "pingus/particles/smoke_particle_holder.hpp"
#include "pingus/particles/snow_particle_holder.hpp"
#include "pingus/pingu.hpp"
#include "pingus/pingu_holder.hpp"
#include "pingus/ecs/components.hpp"
#include "pingus/ecs/systems.hpp"
#include "pingus/object_schema.hpp"
#include "pingus/pingus_level.hpp"
#include "pingus/prefab_file.hpp"
#include "pingus/worldobj_factory.hpp"

namespace pingus {

World::World(PingusLevel const& plf) :
  ambient_light(Color(plf.get_ambient_light())),
  gfx_map(new GroundMap(plf.get_size().width(), plf.get_size().height())),
  game_time(0),
  do_armageddon(false),
  armageddon_count(0),
  game_random(Random::seed_from_string(plf.get_checksum())),
  fx_random(Random::seed_from_string("fx:" + plf.get_checksum())),
  world_obj(),
  pingu_particle_holder(),
  rain_particle_holder(),
  smoke_particle_holder(),
  snow_particle_holder(),
  pingus(new PinguHolder(plf)),
  colmap(gfx_map->get_colmap()),
  gravitational_acceleration(0.2f)
{
  WorldObj::set_world(this);

  log_debug("create particle holder");

  // These get deleted via the world_obj vector in the destructor
  pingu_particle_holder = new pingus::particles::PinguParticleHolder();
  rain_particle_holder  = new pingus::particles::RainParticleHolder();
  smoke_particle_holder = new pingus::particles::SmokeParticleHolder();
  snow_particle_holder  = new pingus::particles::SnowParticleHolder();

  world_obj.push_back(gfx_map);

  world_obj.push_back(pingu_particle_holder);
  world_obj.push_back(rain_particle_holder);
  world_obj.push_back(smoke_particle_holder);
  world_obj.push_back(snow_particle_holder);

  init_worldobjs(plf);
}

void
World::add_object (WorldObj* obj)
{
  world_obj.push_back(obj);
}

namespace {

/** Called for each level object after expanding groups and prefabs, with
    the offset of the prefab the object is part of */
using LevelObjectCallback = std::function<void (std::string const& name, ReaderMapping const& mapping,
                                                Vector2f const& offset, float z_offset)>;

void expand_level_object(std::string const& name, ReaderMapping const& mapping,
                         Vector2f const& offset, float z_offset,
                         LevelObjectCallback const& out)
{
  if (name == "group")
  {
    ReaderCollection collection;
    mapping.read("objects", collection);
    for (auto const& obj : collection.get_objects()) {
      expand_level_object(obj.get_name(), obj.get_mapping(), offset, z_offset, out);
    }
  }
  else if (name == "prefab")
  {
    std::string prefab_name;
    mapping.read("name", prefab_name);

    Vector2f pos;
    float z_index = 0.0f;
    InVector2fZ in_vec{pos, z_index};
    mapping.read("position", in_vec);

    ReaderMapping overrides;
    mapping.read("overrides", overrides);

    PrefabFile prefab = PrefabFile::from_resource(prefab_name);
    for (auto const& obj : prefab.get_objects().get_objects())
    {
      expand_level_object(obj.get_name(), make_override_mapping(obj.get_mapping(), overrides),
                          offset + geom::foffset(pos.x(), pos.y()), z_offset + z_index, out);
    }
  }
  else
  {
    out(name, mapping, offset, z_offset);
  }
}

} // namespace

void
World::init_worldobjs(PingusLevel const& plf)
{
  // Objects are collected first and only turned into entities after
  // sorting, so that entity creation order, and thus system iteration
  // order, follows the z-order like the old WorldObj update order did.
  struct PendingObject
  {
    float z_index;
    WorldObj* obj;
    std::optional<ObjectData> data;
  };

  std::vector<PendingObject> pending;
  for (WorldObj* obj : world_obj) {
    pending.push_back(PendingObject{obj->z_index(), obj, {}});
  }
  world_obj.clear();

  auto add_level_object = [&](std::string const& name, ReaderMapping const& mapping,
                              Vector2f const& offset, float z_offset)
  {
    ObjectTypeDef const* type = ObjectSchema::instance().find(name);
    if (type && systems::is_entity_type(*type))
    {
      ObjectData data = ObjectData::from_reader(*type, name, mapping);
      data.set_pos(data.get_pos() + geom::foffset(offset.x(), offset.y()));
      data.set_z_index(data.get_z_index() + z_offset);
      float const z_index = systems::object_z_index(data);
      pending.push_back(PendingObject{z_index, nullptr, std::move(data)});
    }
    else
    {
      for (WorldObj* obj : WorldObjFactory::instance().create(name, mapping))
      {
        if (obj)
        {
          obj->set_pos(obj->get_pos() + geom::foffset(offset.x(), offset.y()));
          obj->set_z_index(obj->z_index() + z_offset);
          pending.push_back(PendingObject{obj->z_index(), obj, {}});
        }
      }
    }
  };

  for (auto const& reader_object : plf.get_objects().get_objects()) {
    expand_level_object(reader_object.get_name(), reader_object.get_mapping(), Vector2f(), 0.0f, add_level_object);
  }

  // insert a dummy background in case the user didn't provide one
  bool const has_solid_background =
    std::any_of(pending.begin(), pending.end(), [](PendingObject const& p) {
      if (p.obj) {
        return p.obj->is_solid_background();
      } else {
        std::string const& name = p.data->type().name;
        return name == "surface-background" || name == "solidcolor-background";
      }
    });
  if (!has_solid_background)
  {
    auto doc = ReaderDocument::from_string("(solidcolor-background "
                                           "  (position 0 0 -1000) "
                                           "  (colori 127 0 127 255))");
    add_level_object(doc.get_root().get_name(), doc.get_root().get_mapping(), Vector2f(), 0.0f);
  }

  pending.push_back(PendingObject{pingus->z_index(), pingus, {}});

  std::stable_sort(pending.begin(), pending.end(),
                   [](PendingObject const& lhs, PendingObject const& rhs)
                   {
                     return lhs.z_index < rhs.z_index;
                   });

  for (auto& p : pending)
  {
    if (p.obj)
    {
      world_obj.push_back(p.obj);
      object_order.push_back(ObjectRef{p.obj, ecs::null_entity});
    }
    else
    {
      object_order.push_back(ObjectRef{nullptr, systems::create_object(*this, *p.data)});
    }
  }

  // Drawing all world objs to the colmap, gfx, or what ever the
  // objects want to do
  for (auto const& ref : object_order)
  {
    if (ref.obj) {
      ref.obj->on_startup();
    } else {
      systems::startup(*this, ref.entity);
    }
  }
}

World::~World()
{
  for (auto it = world_obj.begin(); it != world_obj.end(); ++it) {
    delete *it;
  }
}

void
World::draw (SceneContext& gc)
{
  WorldObj::set_world(this);

  gc.light().fill_screen(ambient_light);

  for (auto const& ref : object_order)
  {
    if (ref.obj) {
      ref.obj->draw(gc);
    } else {
      systems::draw(*this, gc, ref.entity);
    }
  }
}

void
World::draw_smallmap(SmallMap* smallmap)
{
  WorldObj::set_world(this);

  for(auto obj = world_obj.begin(); obj != world_obj.end(); ++obj)
  {
    (*obj)->draw_smallmap (smallmap);
  }

  systems::draw_smallmap(*this, *smallmap);
}

void
World::update()
{
  WorldObj::set_world(this);

  game_time += 1;

  if (do_armageddon)
  {
    if (game_time % 4 == 0)
    {
      while (armageddon_count < pingus->get_end_id())
      {
        Pingu* pingu = pingus->get_pingu(armageddon_count);

        if (pingu && pingu->get_status() == Pingu::PS_ALIVE)
        {
          pingu->request_set_action(ActionName::BOMBER);
          break;
        }
        else
        {
          ++armageddon_count;
        }
      }

      ++armageddon_count;
    }
  }

  // Traps, exits and other level objects react to the pingus
  systems::update_objects(*this);

  // Let all pingus move and catch each other, update the particles
  for(auto obj = world_obj.begin(); obj != world_obj.end(); ++obj)
  {
    (*obj)->update();
  }

  // Release new pingus
  systems::update_after_pingus(*this);
}

PinguHolder*
World::get_pingus() const
{
  return pingus;
}

int
World::get_width() const
{
  assert(gfx_map);
  return gfx_map->get_width();
}

int
World::get_height() const
{
  assert(gfx_map);
  return gfx_map->get_height();
}

int
World::get_time() const
{
  return game_time;
}

void
World::armageddon(void)
{
  pingus::sound::PingusSound::play_sound("goodidea");
  do_armageddon = true;
  armageddon_count = 0;
}

CollisionMap*
World::get_colmap() const
{
  return colmap;
}

GroundMap*
World::get_gfx_map() const
{
  return gfx_map;
}

void
World::play_sound(std::string const& name, Vector2f const& /* pos */, float volume)
{
  // FIXME: Stereo is for the moment disabled
  /*
    Vector2f center = view->get_center();
    float panning = pos.x - center.x;
    panning /= view->get_width()/2;

    if (panning > 1.0f)
    panning = 1.0f;

    if (panning < -1.0f)
    panning = -1.0f;
  */
  float panning = 0.0f;

  pingus::sound::PingusSound::play_sound(name, volume, panning);
}

Pingu*
World::get_pingu (Vector2f const& pos)
{
  Pingu* current_pingu = nullptr;
  float distance = -1.0;

  for (PinguIter i = pingus->begin(); i != pingus->end(); ++i) {
    if ((*i)->is_over(pos.x(), pos.y()))
    {
      if (distance == -1.0f || distance >= (*i)->dist(pos.x(), pos.y()))
      {
        current_pingu = (*i);
        distance = (*i)->dist(pos.x(), pos.y());
      }
    }
  }

  return current_pingu;
}

float World::get_gravity() const
{
  return gravitational_acceleration;
}

void
World::put(int /* x */, int /* y */, Groundtype::GPType /* p */)
{
}

void
World::put(CollisionMask const& mask, int x, int y, Groundtype::GPType type)
{
  gfx_map->put(mask.get_surface(), x, y);
  colmap->put(mask, x, y, type);
}

void
World::remove(CollisionMask const& mask, int x, int y)
{
  gfx_map->remove(mask.get_surface(), x, y);
  colmap->remove(mask, x, y);
}

WorldObj*
World::get_worldobj(std::string const& id)
{
  for(auto obj = world_obj.begin(); obj != world_obj.end(); ++obj)
  {
    if ((*obj)->get_id() == id)
      return *obj;
  }
  return nullptr;
}

Vector2i
World::get_start_pos(int player_id)
{
  // FIXME: Workaround for lack of start-pos
  Vector2i pos;
  int num_entrances = 0;
  registry.each<components::Transform, components::Owner, components::Entrance>(
    [&](ecs::Entity, components::Transform& transform, components::Owner& owner, components::Entrance&) {
      if (owner.owner_id == player_id)
      {
        pos += geom::ioffset(static_cast<int>(transform.pos.x()),
                             static_cast<int>(transform.pos.y()));
        num_entrances += 1;
      }
    });

  if (num_entrances > 0)
  {
    pos = Vector2i(pos.x() / num_entrances,
                   pos.y() / num_entrances + 100);
  }

  return pos;
}

} // namespace pingus

/* EOF */
