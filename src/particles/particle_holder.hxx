//  $Id: particle_holder.hxx,v 1.4 2002/09/04 17:49:48 grumbel Exp $
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

#ifndef HEADER_PINGUS_PARTICLES_PARTICLE_HOLDER_HXX
#define HEADER_PINGUS_PARTICLES_PARTICLE_HOLDER_HXX

#include <list>
#include "particle_cache.hxx"

class GraphicContext;
class Particle;

/** FIXME: Inherit from std::list<> is evil... */
class ParticleHolder : public std::list<Particle*>
{
private:
  std::vector<Particle*> all_particles;
  ParticleCache     pingu_particles;
  
public:
  ParticleHolder ();
  virtual ~ParticleHolder ();

  void update (float delta);

  void draw_offset (int, int, float); /// const;
  void draw (GraphicContext& gc); /// const;

  void add_pingu_explo (int x, int y);
  void add_particle (Particle* p);
  
  void init_particles ();
  void clear_particles ();
  void add_pingu_particle (int, int, float, float);
  
private:
  ParticleHolder (const ParticleHolder&);
  ParticleHolder operator= (const ParticleHolder&);
};

#endif

/* EOF */
