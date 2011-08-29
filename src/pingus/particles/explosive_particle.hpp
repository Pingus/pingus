//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#if 0
#ifndef HEADER_PINGUS_PINGUS_PARTICLES_EXPLOSIVE_PARTICLE_HPP
#define HEADER_PINGUS_PINGUS_PARTICLES_EXPLOSIVE_PARTICLE_HPP

#include "pingus/particles/particle.hpp"

class ExplosiveParticle : public Particle
{
private:
  bool alive;

public:
  ExplosiveParticle (int x, int y, float x_a, float y_a);
  ~ExplosiveParticle ();
  ///
  void update(float delta);
  ///
  //void draw_offset(int ofx, int ofy, float s);
  ///
  bool is_alive(void);
  void detonate ();

private:
  ExplosiveParticle (const ExplosiveParticle&);
  ExplosiveParticle& operator= (const ExplosiveParticle&);
};

#endif

/* EOF */
#endif
