//  $Id: Cloud.cc,v 1.10 2001/04/10 19:42:58 grumbel Exp $
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

Cloud::Cloud(EntranceData data)
{
  pos = data.pos;
  release_rate = data.release_rate;
  last_release = -release_rate;
  pos.z = 100;
  direction = data.direction;

  surface = PingusResource::load_surface("Entrances/cloud","entrances");
}

void
Cloud::update(float delta)
{
}

void
Cloud::draw_offset(int x, int y, float s)
{
  if (s == 1.0) {
    surface.put_screen(x + pos.x - 115,
		       y + pos.y - 100);
  } else {
    surface.put_screen((int)((pos.x-32 + x) * s),
		       (int)((pos.y-16 + y) * s),
		       s, s);  
  }
}


/* EOF */
