//  $Id: Cloud.cc,v 1.4 2000/06/25 20:22:18 grumbel Exp $
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

#include <cstdlib>
#include <assert.h>

#include "../PingusResource.hh"
#include "../particles/SmokeParticle.hh"
#include "../particles/GroundParticle.hh"
#include "../algo.hh"

#include "Cloud.hh"

Cloud::Cloud(entrance_data data)
{
  x_pos = data.x_pos;
  y_pos = data.y_pos;
  release_rate = data.release_rate;
  last_release = -release_rate;
  z_pos = 100;
  direction = data.direction;

  surface = PingusResource::load_surface("Entrances/cloud","global");
}

void
Cloud::let_move(void)
{
}

void
Cloud::draw_offset(int x, int y, float s)
{
  assert(surface);

  if (s == 1.0) {
    surface->put_screen(x + x_pos - 115,
			y + y_pos - 100);
  } else {
    surface->put_screen((int)((x_pos-32 + x) * s), (int)((y_pos-16 + y) * s),
			s, s);  
  }
}


/* EOF */
