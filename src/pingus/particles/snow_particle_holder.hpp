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

#ifndef HEADER_PINGUS_PINGUS_PARTICLES_SNOW_PARTICLE_HOLDER_HPP
#define HEADER_PINGUS_PINGUS_PARTICLES_SNOW_PARTICLE_HOLDER_HPP

#include <vector>

#include "engine/display/sprite.hpp"
#include "math/vector3f.hpp"
#include "pingus/collision_mask.hpp"
#include "pingus/worldobj.hpp"

class SceneContext;

namespace Particles {

class SnowParticleHolder : public WorldObj
{
private:
  enum ParticleType { Snow1, Snow2, Snow3, Snow4, Snow5 };

  struct SnowParticle {
    bool         alive;
    bool         colliding;
    ParticleType type;
    Vector3f       pos;
    Vector3f       velocity;

    SnowParticle(int x, int y, bool colliding_);
  };

  friend struct SnowParticle;

private:
  Sprite snow1;
  Sprite snow2;
  Sprite snow3;
  Sprite snow4;
  Sprite snow5;
  CollisionMask ground;

  std::vector<SnowParticle> particles;

public:
  SnowParticleHolder ();

  void add_particle (int x, int y, bool colliding = false);

  float get_z_pos () const { return 1000.0f; }
  void set_pos(const Vector3f& p) {}
  Vector3f get_pos() const { return Vector3f(); }

  /// Let the particle move
  void update ();

  /// Draw the particle with the correct zoom resize
  void draw (SceneContext& gc);

private:
  SnowParticleHolder (const SnowParticleHolder&);
  SnowParticleHolder& operator= (const SnowParticleHolder&);
};

} //namespace Particles

#endif

/* EOF */
