//  $Id: ParticleHolder.hh,v 1.5 2000/06/18 17:01:50 grumbel Exp $
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

#ifndef PARTICLEHOLDER_HH
#define PARTICLEHOLDER_HH

#include <list>
#include "Particle.hh"
#include "ParticleCache.hh"

///
class ParticleHolder : public std::list<Particle*>
{
private:
  ///
  std::vector<Particle*> all_particles;
  ///
  ParticleCache     pingu_particles;
  
public:
  ///
  ParticleHolder();
  ///
  virtual ~ParticleHolder();

  ///
  void let_move(void);
  ///
  void draw_offset(int, int, float); /// const;
  void add_pingu_explo(int x, int y);
  ///
  void add_particle(Particle* p);
  
  ///
  void init_particles();
  ///
  void clear_particles();
  ///
  void add_pingu_particle(int, int, float, float);
}///
;

#endif

/* EOF */
