//  $Id: SmokeParticle.cc,v 1.8 2001/03/31 11:21:51 grumbel Exp $
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
#include "../globals.hh"
#include "../PingusResource.hh"
#include "SmokeParticle.hh"

SmokeParticle::SmokeParticle()
{
}

// FIXME: Why the heck do I get an unresolved reference in
// WoodThing.cc when I try to use SmokeParticle there?!
SmokeParticle::SmokeParticle(int x, int y, float x_a, float y_a)
{
  // FIXME: Probably slow??
  if (rand() % 2)
    surface = PingusResource::load_surface("Particles/smoke", "pingus");
  else
    surface = PingusResource::load_surface("Particles/smoke2", "pingus");

  x_pos = x;
  y_pos = y;
  x_add = x_a;
  y_add = y_a;
  livetime = 25 + (rand() % 10);
  time = livetime;
}

void
SmokeParticle::update(void)
{
  x_pos += x_add;
  y_pos += y_add;
  //  y_add += 0.1;

  if (livetime > 0)
    --livetime;
}

void
SmokeParticle::draw_offset(int ofx, int ofy, float s)
{
  if (fast_mode)
    return;

  if (s == 1.0) {
    // FIXME: This segfaults from time to time, don't know why
    surface.put_screen(x_pos + ofx - 16, y_pos + ofy - 16, 3 - (livetime * 4 / time));
  } else {
    int width  = (int)(surface.get_width() * s);
    int height = (int)(surface.get_height() * s);
    surface.put_screen((int)((x_pos + ofx) * s) - width/2, (int)((y_pos + ofy) * s) - height/2,
			width, height, 3 - (livetime * 4 / time));
  }
}

/* EOF */
