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

#ifndef HEADER_PINGUS_PINGUS_PARTICLES_SMOKE_PARTICLE_HOLDER_HPP
#define HEADER_PINGUS_PINGUS_PARTICLES_SMOKE_PARTICLE_HOLDER_HPP

#include <vector>

#include "engine/display/sprite.hpp"
#include "math/vector2f.hpp"
#include "pingus/worldobj.hpp"

class SceneContext;

namespace pingus::particles {

class SmokeParticleHolder : public WorldObj
{
  struct SmokeParticle {
    int    time;
    int    livetime;
    bool   use_surf2;
    Vector2f pos;
    glm::vec2 velocity;

    SmokeParticle(float x, float y, float vel_x, float vel_y);
  };

private:
  Sprite surf1;
  Sprite surf2;

  std::vector<SmokeParticle> particles;

public:
  SmokeParticleHolder();

  void add_particle (float x, float y, float vel_x, float vel_y);

  void set_z_index(float z_index) override {}
  float z_index() const override { return 1000.0f; }
  void set_pos(Vector2f const& p) override {}
  Vector2f get_pos() const override { return Vector2f(); }

  /// Let the particle move
  void update() override;

  /// Draw the particle with the correct zoom resize
  void draw (SceneContext& gc) override;

private:
  SmokeParticleHolder (SmokeParticleHolder const&);
  SmokeParticleHolder& operator= (SmokeParticleHolder const&);
};

} // namespace pingus::particles

#endif

/* EOF */
