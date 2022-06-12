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

#ifndef HEADER_PINGUS_PINGUS_PARTICLES_PINGU_PARTICLE_HOLDER_HPP
#define HEADER_PINGUS_PINGUS_PARTICLES_PINGU_PARTICLE_HOLDER_HPP

#include <vector>

#include "engine/display/sprite.hpp"
#include "math/vector2f.hpp"
#include "pingus/worldobj.hpp"

class SceneContext;

namespace pingus::particles {

class PinguParticleHolder : public WorldObj
{
  struct PinguParticle {
    int  livetime;
    bool use_frame2;
    /// The current position of the particle
    glm::vec2 pos;

    /// The velocity of the particle
    glm::vec2 velocity;

    PinguParticle (int x, int y);
  };

private:
  Sprite surface;
  std::vector<PinguParticle> particles;

public:
  PinguParticleHolder();

  void add_particle (int x, int y);

  void set_z_index(float z_index) override {}
  float z_index() const override { return 1000.0f; }
  void set_pos(Vector2f const& p) override { }
  Vector2f get_pos() const override { return Vector2f(); }

  /// Let the particle move
  void update() override;

  /// Draw the particle with the correct zoom resize
  void draw (SceneContext& gc) override;

private:
  PinguParticleHolder (PinguParticleHolder const&);
  PinguParticleHolder& operator= (PinguParticleHolder const&);
};

} // namespace pingus::particles

#endif

/* EOF */
