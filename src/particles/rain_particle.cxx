//  $Id: rain_particle.cxx,v 1.1 2002/06/12 19:11:31 grumbel Exp $
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

#include "../world.hxx"
#include "../algo.hxx"
#include "../pingus_resource.hxx"
#include "../col_map.hxx"
#include "rain_particle.hxx"

CL_Surface RainParticle::rain1_surf;
CL_Surface RainParticle::rain2_surf;

RainParticle::RainParticle()
{
  alive = true;
  splash = false;
  splash_counter = 0;
}

RainParticle::RainParticle(int arg_x_pos, int arg_y_pos)
{
  pos.x = arg_x_pos;
  pos.y = arg_y_pos;
  splash = false;
  splash_counter = 0;
  alive = true;
  add = 1.0 + frand() * 3.0;
  
  if (!rain1_surf)
    {
      rain1_surf = PingusResource::load_surface("Particles/rain1", "pingus");
      rain2_surf = PingusResource::load_surface("Particles/rain2", "pingus");
    }

  rain_splash = Sprite (PingusResource::load_surface ("Particles/rain_splash", "pingus"), 10.0f, 
			Sprite::NONE, Sprite::ONCE);
  rain_splash.set_align_center_bottom ();

  surface = rain2_surf;
  type = rand () % 3;
}

RainParticle::~RainParticle()
{
}

void
RainParticle::draw_offset(int x_of, int y_of, float s)
{
  if (!splash)
    {
      if (type == 0)
	surface = rain2_surf;
      else
	surface = rain1_surf;	  

      Particle::draw_offset(x_of, y_of - rain1_surf.get_height(), s);
    }
  else
    {
      rain_splash.put_screen(int(pos.x + x_of),
			     int(pos.y + y_of)); 
    }
}

void
RainParticle::update(float delta)
{
  if (splash)
    {
      if (rain_splash.finished ())
	alive = false;

      rain_splash.update (delta);
      ++splash_counter;
      if (splash_counter > 3)
	{
	  splash_counter = 3;
	  alive = false;
	}
    }
  else
    {
      if (world->get_colmap()->getpixel(int(pos.x), int(pos.y)) != ColMap::NOTHING
	  && world->get_colmap()->getpixel(int(pos.x), int(pos.y)) != ColMap::OUTOFSCREEN)
	{
	  splash = true;
	}
      else
	{
	  if (pos.y > world->get_height())
	    alive = false;
    
	  pos.x -= 5.0 * add;
	  pos.y += 16.0 * add;
	}
    }
}

bool 
RainParticle::is_alive()
{
  return alive;
}

/* EOF */
