//  $Id: world.cxx,v 1.47 2003/10/21 11:01:52 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
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

#include <algorithm>
#include <assert.h>
#include <iostream>
#include <typeinfo>
#include "plf.hxx"
#include "pingu_holder.hxx"
#include "sound/sound.hxx"
#include "spot_map.hxx"
#include "world.hxx"
#include "particles/pingu_particle_holder.hxx"
#include "particles/rain_particle_holder.hxx"
#include "particles/smoke_particle_holder.hxx"
#include "particles/snow_particle_holder.hxx"
#include "pingu.hxx"
#include "game_time.hxx"

namespace Pingus {

using Actions::Bomber;

static
bool WorldObj_less (WorldObj* a, WorldObj* b)
{
  return a->get_z_pos () < b->get_z_pos ();
}

World::World(PLF* plf)
  : gfx_map(new PingusSpotMap(plf)),
    game_time(new GameTime (game_speed)),
    do_armageddon(false),
    pingus(new PinguHolder(plf)),
    colmap(gfx_map->get_colmap()),
    gravitational_acceleration(0.25f)
{
  WorldObj::set_world(this);

  // These get deleted via the world_obj vector in the destructor
  pingu_particle_holder = new Particles::PinguParticleHolder();
  rain_particle_holder  = new Particles::RainParticleHolder();
  smoke_particle_holder = new Particles::SmokeParticleHolder();
  snow_particle_holder  = new Particles::SnowParticleHolder();

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
World::init_worldobjs(PLF* plf)
{
  std::vector<WorldObj*> worldobj_d = plf->get_worldobjs();

  for (std::vector<WorldObj*>::iterator i = worldobj_d.begin();
       i != worldobj_d.end ();
       ++i)
    {
      add_object(*i); //->insert_WorldObjs(this);
    }

   world_obj.push_back(pingus);

   std::stable_sort (world_obj.begin (), world_obj.end (), WorldObj_less);

  // Drawing all world objs to the colmap, gfx, or what ever the
  // objects want to do
  for(WorldObjIter obj = world_obj.begin(); obj != world_obj.end(); ++obj)
    {
      (*obj)->on_startup();
    }
}

World::~World()
{
  for (WorldObjIter it = world_obj.begin(); it != world_obj.end(); ++it) {
    delete *it;
  }

  delete game_time;
}

void
World::draw (SceneContext& gc)
{
  WorldObj::set_world(this);

  for(WorldObjIter obj = world_obj.begin(); obj != world_obj.end(); ++obj)
    {
      (*obj)->draw(gc);
    }
}

void
World::draw_smallmap(SmallMap* smallmap)
{
  WorldObj::set_world(this);

  for(WorldObjIter obj = world_obj.begin(); obj != world_obj.end(); ++obj)
    {
      (*obj)->draw_smallmap (smallmap);
    }
}

void
World::update()
{
  WorldObj::set_world(this);

  game_time->update ();

  if (do_armageddon)
    {
      if (game_time->get_ticks() % 4 == 0)
        {
          while (armageddon_count < pingus->get_end_id())
            {
              Pingu* pingu = pingus->get_pingu(armageddon_count);

              if (pingu && pingu->get_status() == PS_ALIVE)
                {
                  pingu->request_set_action(Bomber);
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
  for(WorldObjIter obj = world_obj.begin(); obj != world_obj.end(); ++obj)
    {
      // catch_pingu() is now done in relevant update() if WorldObj
      // needs to catch pingus.
      (*obj)->update();
    }
}

PinguHolder*
World::get_pingus()
{
  return pingus;
}

int
World::get_width()
{
  assert(gfx_map);
  return gfx_map->get_width();
}

int
World::get_height()
{
  assert(gfx_map);
  return gfx_map->get_height();
}

int
World::get_time_passed()
{
  return game_time->get_ticks();
}

void
World::armageddon(void)
{
  Sound::PingusSound::play_sound ("goodidea");
  do_armageddon = true;
  armageddon_count = 0;
}

ColMap*
World::get_colmap()
{
  return colmap;
}

PinguMap*
World::get_gfx_map ()
{
  return gfx_map;
}

void
World::play_sound(std::string name, const Vector& pos, float volume)
{
  // FIXME: Stereo is for the moment disabled
  /*
    Vector center = view->get_center();
    float panning = pos.x - center.x;
    panning /= view->get_width()/2;

    if (panning > 1.0f)
    panning = 1.0f;

    if (panning < -1.0f)
    panning = -1.0f;
  */
  float panning = 0.0f;

  Sound::PingusSound::play_sound(name, volume, panning);
}

Pingu*
World::get_pingu (const Vector& pos)
{
  Pingu* current_pingu = 0;
  double distance = -1.0;

  for (PinguIter i = pingus->begin (); i != pingus->end (); ++i) {
    if ((*i)->is_over(int(pos.x), int(pos.y)))
      {
	      if (distance == -1.0f || distance >= (*i)->dist((int) pos.x, (int)pos.y))
	        {
	          current_pingu = (*i);
	          distance = (*i)->dist((int)pos.x, (int)pos.y);
	        }
      }
  }

  return current_pingu;
}

GameTime*
World::get_game_time ()
{
  return game_time;
}

float World::get_gravity()
{
  return gravitational_acceleration;
}

} // namespace Pingus

/* EOF */
