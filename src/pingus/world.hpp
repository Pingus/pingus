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

#ifndef HEADER_PINGUS_PINGUS_WORLD_HPP
#define HEADER_PINGUS_PINGUS_WORLD_HPP

#include <string>
#include <vector>

#include "math/vector2i.hpp"
#include "pingus/collision_mask.hpp"
#include "pingus/groundtype.hpp"
#include "math/vector2f.hpp"
#include "fwd.hpp"

namespace pingus {

/** The World holds all objects of the pingu enviroment.

    It holds the pingus, traps, exits, entrances, etc.. It keeps
    control that all objects become time to move and can catch each
    other. */
class World
{
private:
  Color ambient_light;

  /** groundmap for the pingus */
  GroundMap* gfx_map;

  /** manager class for the time in the pingus world */
  int game_time;

  /** set to true once an armageddon got started */
  bool do_armageddon;

  /** FIXME: ugly hack to iterate over all pingus and make bombers out
      of them, should use pingus_id instead */
  unsigned int armageddon_count;

  std::vector<WorldObj*> world_obj;

  pingus::particles::PinguParticleHolder* pingu_particle_holder;
  pingus::particles::RainParticleHolder*  rain_particle_holder;
  pingus::particles::SmokeParticleHolder* smoke_particle_holder;
  pingus::particles::SnowParticleHolder*  snow_particle_holder;
  PinguHolder*                    pingus;

  // Pointers which are references to objects from other classes
  CollisionMap*         colmap;

  void    init_worldobjs (PingusLevel const& plf);

  /** Acceleration due to gravity in the world */
  const float gravitational_acceleration;

public:
  World(PingusLevel const& level);
  virtual ~World();

  /** Add an object to the world, obj needs to be new'ed the World
      make sure that it will get deleted */
  void add_object (WorldObj* obj);

  /** Draw the world onto the given SceneContext */
  void    draw (SceneContext& gc);

  /** Draw the world onte the given SmallMap*/
  void    draw_smallmap(SmallMap* smallmap);

  /** Update the World */
  void    update();

  /** Issue an armageddon, all Pingus will explode in some seconds. */
  void    armageddon();

  /** @return The absolute height of the world. */
  int     get_height() const;
  /** @return The absolute width of the world */
  int     get_width() const;

  /** Returns the time passed since the level was started */
  int get_time() const;

  /** @return A pointer to the collision map used in this world */
  CollisionMap* get_colmap() const;

  /** @return A pointer to the gfx map used in this world */
  GroundMap* get_gfx_map() const;

  void put(int x, int y, Groundtype::GPType p = Groundtype::GP_GROUND);
  void put(CollisionMask const&, int x, int y, Groundtype::GPType);

  void remove(CollisionMask const&, int x, int y);

  WorldObj* get_worldobj(std::string const& id);

  /** @return A pointer to the worlds pingu particle holder */
  pingus::particles::PinguParticleHolder* get_pingu_particle_holder() { return pingu_particle_holder; }

  /** @return A pointer to the worlds rain particle holder */
  pingus::particles::RainParticleHolder* get_rain_particle_holder() { return rain_particle_holder; }

  /** @return A pointer to the worlds smoke particle holder */
  pingus::particles::SmokeParticleHolder* get_smoke_particle_holder() { return smoke_particle_holder; }

  /** @return A pointer to the worlds snow particle holder */
  pingus::particles::SnowParticleHolder* get_snow_particle_holder() { return snow_particle_holder; }

  /** @return true if the world is currently doing an armageddon */
  bool check_armageddon() const { return do_armageddon; }

  /** Play a sound as if it would have been generated at the given
      position, adjust panning and volume by the position relative to
      the center of the screen
      @param name The name of the sound to play ("ohno", not "sounds/ohno.wav")
      @param pos Position from which the sound seems to come (z-pos is
      going to be ignored) void play_sound (std::string name, const
      @param volume The volume of the sound
  */
  void play_sound (std::string const& name, Vector2f const& pos, float volume = 0.5f);

  PinguHolder* get_pingus(void) const;

  /** @return the pingu at the given word coordinates, 0 if none is there */
  Pingu* get_pingu (Vector2f const& pos);

  /** Get the acceleration due to gravity in the world */
  float get_gravity() const;

  /** Returns the start pos for the given player */
  Vector2i get_start_pos(int player_id) const;

private:
  World (World const&);
  World& operator= (World const&);
};

} // namespace pingus

#endif

/* EOF */
