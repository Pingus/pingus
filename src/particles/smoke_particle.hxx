//  $Id: smoke_particle.hxx,v 1.5 2002/09/27 11:26:49 torangan Exp $
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

#ifndef HEADER_PINGUS_PARTICLES_SMOKE_PARTICLE_HXX
#define HEADER_PINGUS_PARTICLES_SMOKE_PARTICLE_HXX

#include "particle.hxx"

class SmokeParticle : public Particle
{
private:
  int time;
  static CL_Surface surf1;
  static CL_Surface surf2;
  
public:
  SmokeParticle ();
  SmokeParticle (int, int, float, float);

  void update (float delta);
  void draw_offset (int ofx, int ofy, float s);
  
private:
  SmokeParticle (const SmokeParticle&);
  SmokeParticle& operator= (const SmokeParticle&);
};

#endif

/* EOF */
