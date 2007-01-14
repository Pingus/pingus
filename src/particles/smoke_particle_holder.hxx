//  $Id: smoke_particle_holder.hxx,v 1.5 2003/10/19 12:25:47 grumbel Exp $
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

#ifndef HEADER_PINGUS_PARTICLES_SMOKE_PARTICLE_HOLDER_HXX
#define HEADER_PINGUS_PARTICLES_SMOKE_PARTICLE_HOLDER_HXX

#include <vector>
#include <ClanLib/Display/sprite.h>
#include "../vector.hxx"
#include "../worldobj.hxx"


class SceneContext;

namespace Particles {

class SmokeParticleHolder : public WorldObj
{
  struct SmokeParticle {
    int    time;
    int    livetime;
    bool   use_surf2;
    Vector pos;
    Vector velocity;

    SmokeParticle(float x, float y, float vel_x, float vel_y);
  };

private:
  CL_Sprite surf1;
  CL_Sprite surf2;

  std::vector<SmokeParticle> particles;

public:
  SmokeParticleHolder ();

  void add_particle (float x, float y, float vel_x, float vel_y);

  float get_z_pos () const { return 1000.0f; }

  /// Let the particle move
  void update ();

  /// Draw the particle with the correct zoom resize
  void draw (SceneContext& gc);

private:
  SmokeParticleHolder (const SmokeParticleHolder&);
  SmokeParticleHolder& operator= (const SmokeParticleHolder&);
};

} // namespace Particles

#endif

/* EOF */
