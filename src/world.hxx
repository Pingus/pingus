//  $Id: world.hxx,v 1.29 2003/10/18 23:17:27 grumbel Exp $
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

#ifndef HEADER_PINGUS_WORLD_HXX
#define HEADER_PINGUS_WORLD_HXX

#include "pingus.hxx"
#include <vector>
#include <string>

namespace Pingus {

// Forward declarations
class Vector;
class ActionHolder;
class Entrance;
class Exit;
class Hotspot;
class Liquid;
class PinguHolder;
class PinguMap;
class PingusLevel;
class Pingu;
class Trap;
class View;
class ColMap;
class WorldObj;
class GameTime;
class SmallMap;
class SceneContext;

namespace Particles {
  class PinguParticleHolder;
  class RainParticleHolder;
  class SmokeParticleHolder;
  class SnowParticleHolder;
}

/** The World holds all objects of the pingu enviroment.

    It holds the pingus, traps, exits, entrances, etc.. It keeps
    control that all objects become time to move and can catch each
    other. */
class World
{
private:
  /** groundmap for the pingus */
  PinguMap* gfx_map;

  /** manager class for the time in the pingus world */
  GameTime* game_time;

  /** set to true once an armageddon got started */
  bool do_armageddon;

  /** FIXME: ugly hack to iterate over all pingus and make bombers out
      of them, should use pingus_id instead */
  unsigned int armageddon_count;

  std::vector<WorldObj*> world_obj;
  typedef std::vector<WorldObj*>::iterator WorldObjIter;

  // These pointers hold objects and must be deleted
  Particles::PinguParticleHolder* pingu_particle_holder;
  Particles::RainParticleHolder*  rain_particle_holder;
  Particles::SmokeParticleHolder* smoke_particle_holder;
  Particles::SnowParticleHolder*  snow_particle_holder;
  PinguHolder*                    pingus;

  // Pointers which are references to objects from other classes
  ActionHolder*   action_holder;
  ColMap*         colmap;

  void    init_worldobjs (const PingusLevel& plf);

  /** Acceleration due to gravity in the world */
  const float gravitational_acceleration;

public:
  World(const PingusLevel& level);
  virtual ~World();

  /** Add an object to the world, obj needs to be new'ed the World
      make sure that it will get deleted */
  void add_object (WorldObj* obj);

  /** Draw the world onto the given SceneContext */
  void    draw (SceneContext& gc);

  /** Draw the world onte the given SmallMap*/
  void    draw_smallmap(SmallMap* smallmap);

  /** Update the World */
  void    update ();

  /** Issue an armageddon, all Pingus will explode in some seconds. */
  void    armageddon ();

  /** @return The absolute height of the world. */
  int     get_height ();
  /** @return The absolute width of the world */
  int     get_width();

  /** Returns the time passed since the level was started */
  int get_time_passed();

  /** @return A pointer to the collision map used in this world */
  ColMap* get_colmap();

  /** @return A pointer to the gfx map of this world */
  PinguMap* get_gfx_map();

  /** @return A pointer to the worlds pingu particle holder */
  Particles::PinguParticleHolder* get_pingu_particle_holder () { return pingu_particle_holder; }

  /** @return A pointer to the worlds rain particle holder */
  Particles::RainParticleHolder* get_rain_particle_holder () { return rain_particle_holder; }

  /** @return A pointer to the worlds smoke particle holder */
  Particles::SmokeParticleHolder* get_smoke_particle_holder () { return smoke_particle_holder; }

  /** @return A pointer to the worlds snow particle holder */
  Particles::SnowParticleHolder* get_snow_particle_holder () { return snow_particle_holder; }

    /** @return true if the world is currently doing an armageddon */
  bool check_armageddon() { return do_armageddon; }

  /** Play a sound as if it would have been generated at the given
      position, adjust panning and volume by the position relative to
      the center of the screen
      @param name The name of the sound to play ("ohno", not "sounds/ohno.wav")
      @param pos Position from which the sound seems to come (z-pos is
      going to be ignored) void play_sound (std::string name, const
      @param volume The volume of the sound
  */
  void play_sound (std::string name, const Vector& pos, float volume = 0.5f);

  PinguHolder* get_pingus(void);

  /** @return the pingu at the given word coordinates, 0 if none is there */
  Pingu* get_pingu (const Vector& pos);

  /** Return a pointer to the GameTime object of this World */
  GameTime* get_game_time ();

  /** Get the acceleration due to gravity in the world */
  float get_gravity();

private:
  World (const World&);
  World& operator= (const World&);
};

} // namespace Pingus

#endif

/* EOF */
