//  $Id: snow_particle.hxx,v 1.4 2002/09/27 11:26:49 torangan Exp $
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

#ifndef HEADER_PINGUS_PARTICLES_SNOW_PARTICLE_HXX
#define HEADER_PINGUS_PARTICLES_SNOW_PARTICLE_HXX

#include "particle.hxx"

class SnowParticle : public Particle
{
public:
  SnowParticle ();
  SnowParticle (int x, int y);
  void init (int x, int y);
  virtual ~SnowParticle ();

  virtual void update (float delta);
  virtual bool is_alive ();
  
private:
  SnowParticle (const SnowParticle&); 
  SnowParticle& operator= (const SnowParticle&);
};

class CollidingSnowParticle : public SnowParticle
{
private:
  bool alive;
  static CL_Surface ground_snow;
  
public:
  CollidingSnowParticle ();
  CollidingSnowParticle (int x, int y);
  virtual ~CollidingSnowParticle ();

  virtual void update (float delta);
  virtual bool is_alive ();  
  
private:
  CollidingSnowParticle (const CollidingSnowParticle&);
  CollidingSnowParticle& operator= (const CollidingSnowParticle&);
};

#endif

/* EOF */
