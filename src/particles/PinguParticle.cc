//  $Id: PinguParticle.cc,v 1.2 2000/02/09 21:43:44 grumbel Exp $
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

#include "../algo.hh"
#include "../PingusResource.hh"
#include "PinguParticle.hh"

PinguParticle::PinguParticle()
{
  surface = CL_Surface::load("Particles/particle", PingusResource::get("pingus.dat"));
  livetime = 50 + (rand() % 25);
  size  = 1.0;
  size_add = (frand() - 0.2) / 35;
}

PinguParticle::PinguParticle(int x, int y, double x_a, double y_a)
{
  x_pos = x;
  y_pos = y;
  x_add = x_a;
  y_add = y_a;
  size  = 1.0;
  size_add = (frand() - 0.2) / 35;
  livetime = 50 + (rand() % 25);
  surface = CL_Surface::load("Particles/particle", PingusResource::get("pingus.dat"));
}

void
PinguParticle::load_data()
{
}

void
PinguParticle::draw_offset(int ofx, int ofy, double s) const
{
  surface->put_screen(x_pos + ofx, y_pos + ofy);
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
PinguParticle::let_move()
{
  // Comment this to get unzoomed particles
  size += size_add;
  
  x_pos += x_add * size;
  y_pos += y_add * size;
  y_add += 0.1;

  if (livetime > 0)
    --livetime;
}

/* EOF */
