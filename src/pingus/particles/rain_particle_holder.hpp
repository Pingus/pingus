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

#ifndef HEADER_PINGUS_PINGUS_PARTICLES_RAIN_PARTICLE_HOLDER_HPP
#define HEADER_PINGUS_PINGUS_PARTICLES_RAIN_PARTICLE_HOLDER_HPP

#include <vector>

#include "engine/display/sprite.hpp"
#include "math/vector3f.hpp"
#include "pingus/worldobj.hpp"

class GraphicContext;

namespace Particles {

class RainParticleHolder : public WorldObj
{
  struct RainParticle {
    bool  alive;
    bool  splash;
    bool  use_rain2_surf;
    int   splash_counter;
    float splash_frame;

    // pos.z contains a modificator for x and y pos
    Vector3f pos;

    RainParticle(int x, int y);
  };

private:
  Sprite rain1_surf;
  Sprite rain2_surf;
  Sprite rain_splash;

  std::vector<RainParticle> particles;

public:
  RainParticleHolder ();

  void add_particle(int x, int y);

  float get_z_pos () const { return 1000.0f; }
  void set_pos(const Vector3f& p) { }
  Vector3f get_pos() const { return Vector3f(); }

  /// Let the particle move
  void update ();

  /// Draw the particle with the correct zoom resize
  void draw (SceneContext& gc);

private:
  RainParticleHolder (const RainParticleHolder&);
  RainParticleHolder& operator= (const RainParticleHolder&);
};

} // namespace Particles

#endif

/* EOF */
