//  $Id: rain_particle_holder.hxx,v 1.2 2002/12/31 15:09:33 torangan Exp $
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

#ifndef HEADER_PINGUS_PARTICLES_RAIN_PARTICLE_HOLDER_HXX
#define HEADER_PINGUS_PARTICLES_RAIN_PARTICLE_HOLDER_HXX

#include <vector>
#include <ClanLib/Display/Display/surface.h>
#include "../vector.hxx"
#include "../worldobj.hxx"

class GraphicContext;

class RainParticleHolder : public WorldObj
{
  struct RainParticle {
    bool  alive;    
    bool  splash;
    bool  use_rain2_surf;
    int   splash_counter;
    float splash_frame;

    // pos.z contains a modificator for x and y pos
    Vector pos;

    RainParticle(int x, int y);
  };

private:
  CL_Surface rain1_surf;
  CL_Surface rain2_surf;
  CL_Surface rain_splash;
  float      world_width; // float since it's compared to a float value

  std::vector<RainParticle> particles;
  
public:
  RainParticleHolder ();

  void add_particle(int x, int y);

  float get_z_pos () const { return 1000.0f; }

  /// Let the particle move
  void update ();

  /// Draw the particle with the correct zoom resize
  void draw (GraphicContext& gc);

  /** sets the width of the current world so that no particles
    * are drawn outside the visible area (optimization).
    * If this method is not called a default value of 1000 will
    * be used to ensure that particles are drawn. */
  void set_world_width(int width);

private:
  RainParticleHolder (const RainParticleHolder&);
  RainParticleHolder& operator= (const RainParticleHolder&);
};

#endif

/* EOF */
