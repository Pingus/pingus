//  $Id: RainParticle.cc,v 1.2 2000/12/14 21:35:56 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#include "../algo.hh"
#include "../PingusResource.hh"
#include "Particle.hh"
#include "RainParticle.hh"

CL_Surface RainParticle::rain_surf;
CL_Surface RainParticle::rain_splash;

RainParticle::RainParticle()
{
  alive = true;
  splash = false;
  splash_counter = 0;
}

RainParticle::RainParticle(int arg_x_pos, int arg_y_pos)
{
  x_pos = arg_x_pos;
  y_pos = arg_y_pos;
  splash = false;
  splash_counter = 0;
  alive = true;
  add = 1.0 + frand() * 3.0;
  
  if (!rain_surf)
    {
      rain_surf = PingusResource::load_surface("Particles/rain1", "pingus");
      rain_splash = PingusResource::load_surface("Particles/rain_splash", "pingus");
    }
  surface = rain_surf;
}

RainParticle::~RainParticle()
{
}

void
RainParticle::draw_offset(int x_of, int y_of, float s)
{
  //  std::cout << "draw_offset: " << splash << std::endl;
  
  if (!splash)
    {
      Particle::draw_offset(x_of, y_of - rain_surf.get_height(), s);
    }
  else
    {
      rain_splash.put_screen((int)x_pos + x_of - rain_splash.get_width()/2, 
			     (int)y_pos + y_of - rain_splash.get_height(),
			     splash_counter);
    }
}

void
RainParticle::let_move()
{
  if (splash)
    {
      ++splash_counter;
      if (splash_counter > 3)
	{
	  splash_counter = 3;
	  alive = false;
	}
    }
  else
    {
      if (world->get_colmap()->getpixel(x_pos, y_pos) != ColMap::NOTHING
	  && world->get_colmap()->getpixel(x_pos, y_pos) != ColMap::OUTOFSCREEN)
	{
	  splash = true;
	}
      else
	{
	  if (y_pos > world->get_height())
	    alive = false;
    
	  x_pos -= 5.0 * add;
	  y_pos += 16.0 * add;
	}
    }
}

bool 
RainParticle::is_alive()
{
  return alive;
}

/* EOF */
