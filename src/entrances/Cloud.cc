//  $Id: Cloud.cc,v 1.1 2000/02/11 21:22:11 grumbel Exp $
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

  surface = CL_Surface::load("Entrances/cloud", PingusResource::get("global.dat"));
}

void
Cloud::let_move(void)
{
}

void
Cloud::draw_offset(int x, int y, float s)
{
  if (s == 1.0) {
    surface->put_screen(x + x_pos - 115,
			y + y_pos - 100);
  } else {
    surface->put_screen((int)((x_pos-32 + x) * s), (int)((y_pos-16 + y) * s),
			s, s);  
  }
}


/* EOF */
