//  $Id: pingu_particle.cxx,v 1.1 2002/06/12 19:11:31 grumbel Exp $
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

#include "../col_map.hxx"
#include "../world.hxx"
#include "../algo.hxx"
#include "../pingus_resource.hxx"
#include "pingu_particle.hxx"

static const float x_collision_decrease = 0.3f;
static const float y_collision_decrease = 0.6f;

CL_Surface PinguParticle::sur;

PinguParticle::PinguParticle()
{
  surface = PingusResource::load_surface("Particles/particle", "pingus");
  livetime = 50 + (rand() % 25);
  size  = 1.0;
  size_add = (frand() - 0.2) / 35;
}

PinguParticle::PinguParticle(int x, int y, float x_a, float y_a)
  : Particle (x, y, x_a, y_a)
{
  size  = 1.0;
  size_add = (frand() - 0.2) / 35;
  livetime = 50 + (rand() % 50);

  if (sur)
    surface = sur;
  else
    sur = PingusResource::load_surface("Particles/particle", "pingus");
}

void
PinguParticle::init(int x, int y, float x_a, float y_a)
{
  pos.x = x;
  pos.y = y;
  velocity.x = x_a;
  velocity.y = y_a;
  size  = 1.0;
  size_add = (frand() - 0.2) / 35;
  livetime = 50 + (rand() % 25);
}

void
PinguParticle::draw_offset(int ofx, int ofy, float /*s*/)
{
  surface.put_screen(int(pos.x + ofx), int(pos.y + ofy));
  /* Particle resizeing is disabled, because it is to slow
  if (s * size == 1.0) {
    surface->put_screen(x_pos + ofx, y_pos + ofy);
  } else {
    int width  = (int)(surface->get_width() * s * size);
    int height = (int)(surface->get_height() * s * size);
    surface->put_screen((int)((x_pos + ofx) * s) - width/2, (int)((y_pos + ofy) * s) - height/2,
			width, height);
  }
  */
}

void
PinguParticle::update(float /*delta*/)
{
  float tmp_x_add = 0.0;
  float tmp_y_add = 0.0;
  
  // Simulated gravity
  velocity.y += 0.2f;
  
  if (velocity.y > 0)
    {
      for (tmp_y_add = velocity.y; tmp_y_add >= 1.0; tmp_y_add -= 1.0)
	{
	  if (world->get_colmap()->getpixel((int)pos.x, (int)pos.y)) 
	    {
	      velocity.y = velocity.y * -y_collision_decrease;
	      tmp_y_add = -tmp_y_add;
	      pos.y -= 1.0;
	      break;
	    }
	  pos.y += 1.0;
	}
      pos.y += tmp_y_add;
    }
  else
    {
      for (tmp_y_add = velocity.y; tmp_y_add <= -1.0; tmp_y_add += 1.0)
	{
	  if (world->get_colmap()->getpixel((int)pos.x, (int)pos.y)) {
	    velocity.y = velocity.y * -y_collision_decrease;
	    tmp_y_add = -tmp_y_add;
	    pos.y += 1.0;
	    break;
	  }
	  pos.y -= 1.0;
	}
      pos.y += tmp_y_add;
    }

  if (velocity.x > 0)
    {
      for (tmp_x_add = velocity.x; tmp_x_add >= 1.0; tmp_x_add -= 1.0)
	{
	  if (world->get_colmap()->getpixel((int)pos.x, (int)pos.y)) {
	    velocity.x = velocity.x * -x_collision_decrease;
	    tmp_x_add = -tmp_x_add;
	    pos.x -= 1.0;
	    break;
	  }
	  pos.x += 1.0;
	}
      pos.x += tmp_x_add;
    }
  else
    {
      for (tmp_x_add = velocity.x; tmp_x_add <= -1.0; tmp_x_add += 1.0)
	{
	  if (world->get_colmap()->getpixel((int)pos.x, (int)pos.y)) {
	    velocity.x = velocity.x * -x_collision_decrease;
	    tmp_x_add = -tmp_x_add;
	    pos.x += 1.0;
	    break;
	  }
	  pos.x -= 1.0;
	}
      pos.x += tmp_x_add;
    }

  // Simple physics
#if 0
  size += size_add;
  
  pos.x += x_add * size;
  pos.y += y_add * size;
  y_add += 0.1;
#endif

  if (livetime > 0)
    --livetime;
}

/* EOF */
