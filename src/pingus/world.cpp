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
#include "pingus/pingus_level.hpp"
#include "pingus/worldobj_factory.hpp"
#include "pingus/worldobjs/entrance.hpp"

namespace pingus {

World::World(PingusLevel const& plf) :
  ambient_light(Color(plf.get_ambient_light())),
  gfx_map(new GroundMap(plf.get_size().width(), plf.get_size().height())),
  game_time(0),
  do_armageddon(false),
  armageddon_count(0),
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

void
World::init_worldobjs(PingusLevel const& plf)
{
  for (auto const& reader_object : plf.get_objects().get_objects())
  {
    std::vector<WorldObj*> objs = WorldObjFactory::instance().create(reader_object);
    for(auto obj = objs.begin(); obj != objs.end(); ++obj)
    {
      if (*obj)
      {
        add_object(*obj);
      }
    }
  }

  {
    // insert a dummy background in case the user didn't provide one
    if (std::none_of(world_obj.begin(), world_obj.end(),
                     [](WorldObj* obj) { return obj->is_solid_background(); }))
    {
      auto doc = ReaderDocument::from_string("(solidcolor-background "
                                             "  (position 0 0 -1000) "
                                             "  (colori 127 0 127 255))");
      auto objs = WorldObjFactory::instance().create(doc.get_root());
      for(auto obj = objs.begin(); obj != objs.end(); ++obj)
      {
        add_object(*obj);
      }
    }
  }

  world_obj.push_back(pingus);

  std::stable_sort(world_obj.begin(), world_obj.end(),
                   [](WorldObj* lhs, WorldObj* rhs)
                   {
                     return lhs->z_index() < rhs->z_index();
                   });

  // Drawing all world objs to the colmap, gfx, or what ever the
  // objects want to do
  for(auto obj = world_obj.begin(); obj != world_obj.end(); ++obj)
  {
    (*obj)->on_startup();
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

  for(auto obj = world_obj.begin(); obj != world_obj.end(); ++obj)
  {
    (*obj)->draw(gc);
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

  // Let all pingus move and
  // Let the pingus catch each other and
  // Let the traps catch the pingus and
  // Let the exit catch the pingus
  for(auto obj = world_obj.begin(); obj != world_obj.end(); ++obj)
  {
    // catch_pingu() is now done in relevant update() if WorldObj
    // needs to catch pingus.
    (*obj)->update();
  }
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
World::play_sound(std::string const& name, Vector2f const& pos, float volume)
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
World::put(int x, int y, Groundtype::GPType p)
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
World::get_start_pos(int player_id) const
{
  // FIXME: Workaround for lack of start-pos
  Vector2i pos;
  int num_entrances = 0;
  for(auto obj = world_obj.begin(); obj != world_obj.end(); ++obj)
  {
    pingus::worldobjs::Entrance* entrance = dynamic_cast<pingus::worldobjs::Entrance*>(*obj);
    if (entrance && entrance->get_owner_id() == player_id)
    {
      pos += geom::ioffset(static_cast<int>(entrance->get_pos().x()),
                           static_cast<int>(entrance->get_pos().y()));
      num_entrances += 1;
    }
  }

  if (num_entrances > 0)
  {
    pos = Vector2i(pos.x() / num_entrances,
                   pos.y() / num_entrances + 100);
  }

  return pos;
}

} // namespace pingus

/* EOF */
