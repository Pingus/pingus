
//  $Id: particle.hxx,v 1.3 2002/08/23 15:49:56 torangan Exp $
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

#ifndef HEADER_PINGUS_PARTICLES_PARTICLE_HXX
#define HEADER_PINGUS_PARTICLES_PARTICLE_HXX

#include <ClanLib/Core/Math/cl_vector.h>
#include <ClanLib/Display/Display/surface.h>

#include "../worldobj.hxx"

class Particle : public WorldObj
{
protected:
  CL_Surface surface;
  
  /// The current position of the particle
  CL_Vector pos;
  
  /// The velocity of the particle
  CL_Vector velocity;

  CL_Vector force;
  int livetime;
public:
  Particle();
  Particle(int x, int y, float x_a, float y_a);
  virtual ~Particle();

  /// Reinit a allready created particle with now coordinates
  virtual void init(int x, int y, float x_a, float y_a);

  /** If false is returned the particle gets deleted by the
      ParticleHolder */
  virtual bool is_alive(void);

  virtual float get_z_pos() const { return pos.z; }

  /// Let the particle move
  virtual void update(float delta);

  /// Draw the particle with the correct zoom resize
  virtual void draw_offset(int, int, float);
  
private:
  Particle (const Particle&);
  Particle operator= (const Particle&);
};

#endif

/* EOF */
