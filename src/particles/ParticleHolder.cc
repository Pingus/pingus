//  $Id: ParticleHolder.cc,v 1.5 2000/06/11 15:23:31 grumbel Exp $
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

#include "../globals.hh"
#include "../algo.hh"
#include "PinguParticle.hh"

#include "ParticleHolder.hh"

ParticleHolder::ParticleHolder()
{
  init_particles();
}

ParticleHolder::~ParticleHolder()
{
  std::cout << "ParticleHolder:~ParticleHolder" << std::endl;
}

void
ParticleHolder::let_move()
{
  for(std::list<Particle*>::iterator i = this->begin(); i != this->end(); i++) 
    {
      if ((*i)->is_alive()) 
	{
	  (*i)->let_move();
	} 
      else 
	{
	  i = this->erase(i);
	  i--;
	}
  }
}

void
ParticleHolder::draw_offset(int x, int y, float s) // const
{
  for(std::list<Particle*>::iterator i = this->begin(); i != this->end(); i++) 
    { 
      (*i)->draw_offset(x, y, s);
    }
}

void
ParticleHolder::add_pingu_explo(int x, int y)
{
  for(int i=0; i < 50; i++) 
    {
      add_pingu_particle(x, y, frand()*7 - 3.5, frand()* -7.0);
    }
}

void
ParticleHolder::add_particle(Particle* p)
{
  if (verbose > 1) std::cout << "--- ParticleHolder: Warrning add_particle() is slow" << std::endl;
  this->push_back(p);
}

void
ParticleHolder::add_pingu_particle(int x, int y, float vx,  float vy)
{
  Particle* p;
  
  p = pingu_particles.create();
  p->init(x, y, vx, vy);

  this->push_back(p);
}

void
ParticleHolder::init_particles()
{
  pingu_particles.allocate(400);
}

void 
ParticleHolder::clear_particles()
{
  std::cout << "ParticleHolder:: Clearing particles" << std::endl;
  for(std::vector<Particle*>::iterator i = all_particles.begin(); i != all_particles.end(); i++)
    {
      delete (*i);
    }
}

/* EOF */
