//  $Id: SnowParticle.cc,v 1.12 2001/04/03 10:45:51 grumbel Exp $
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

#include <ClanLib/core.h>
#include "../World.hh"
#include "../PingusResource.hh"
#include "../algo.hh"
#include "SnowParticle.hh"

SnowParticle::SnowParticle()
{
}

SnowParticle::SnowParticle(int x, int y)
{
  init(x, y);
}

void
SnowParticle::init(int x, int y)
{
  CL_Surface snow;

  x_pos = x;
  y_pos = y;
  x_add = 0.0;

  switch (rand() % 10)
    {
    case 0:
      snow = PingusResource::load_surface("Particles/snow1", "pingus");
      break;
    case 1:
      snow = PingusResource::load_surface("Particles/snow2", "pingus");
      break;
    case 2:
    case 3:
      snow = PingusResource::load_surface("Particles/snow3", "pingus");
      break;
    case 5:
    case 6:
      snow = PingusResource::load_surface("Particles/snow4", "pingus");      
    default:
      snow = PingusResource::load_surface("Particles/snow5", "pingus");
      break;
    }

  surface = snow;  
  
  y_add = 1.0 + (frand() * 3.5);
}

SnowParticle::~SnowParticle()
{
}

void
SnowParticle::update(float delta)
{
  y_pos += y_add;
  x_pos += x_add;
  x_add += (frand() - 0.5) / 10;
}

bool
SnowParticle::is_alive()
{
  if (y_pos < world->get_height())
    return true;
  else
    return false;
}

CL_Surface CollidingSnowParticle::ground_snow;

CollidingSnowParticle::CollidingSnowParticle()
{
  
}

CollidingSnowParticle::CollidingSnowParticle(int x, int y)
{
  SnowParticle::init(x, y);
  alive = true;
  if (!ground_snow)
    ground_snow = PingusResource::load_surface("Particles/ground_snow", "pingus");
}

CollidingSnowParticle::~CollidingSnowParticle()
{
}

void 
CollidingSnowParticle::update(float delta)
{
  assert(ground_snow);
  SnowParticle::update(delta);

  int pixel = world->get_colmap()->getpixel(x_pos, y_pos);

  if (pixel != ColMap::NOTHING && pixel != ColMap::LAVA
      && pixel != ColMap::WATER && pixel != ColMap::OUTOFSCREEN)
    {
      //std::cout << "Snow: touch down: " << x_pos << " " << y_pos << std::endl;
      world->get_gfx_map()->put(ground_snow,
				(int)x_pos - 1,
				(int)y_pos - 1);
      alive = false;
    }
}

bool 
CollidingSnowParticle::is_alive()
{
  return alive && SnowParticle::is_alive();
}

/* EOF */
