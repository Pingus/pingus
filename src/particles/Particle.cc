//  $Id: Particle.cc,v 1.12 2002/06/01 18:05:37 torangan Exp $
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

#include <cassert>
#include "Particle.hh"

Particle::Particle()
{
}

// Set all parameters to theire default value
Particle::Particle(int x, int y, float x_a, float y_a)
  : pos (x, y),
    velocity (x_a, y_a)
{
  livetime = 50 + ( rand() % 25);
}

Particle::~Particle()
{
  
}

void
Particle::init(int x, int y, float x_a, float y_a)
{
  pos.x = x;
  pos.y = y;
  velocity.x = x_a;
  velocity.y = y_a;
  livetime = 50 + ( rand() % 25);
}

void
Particle::update(float /*delta*/)
{
  pos.x += velocity.x;
  pos.y += velocity.y;
  velocity.y += 0.1f;

  if (livetime > 0)
    --livetime;
}

void
Particle::draw_offset(int ofx, int ofy, float s)
{
  if (s == 1.0) {
    surface.put_screen((int)pos.x + ofx, (int)pos.y + ofy);
  } else {
    int width  = (int)(surface.get_width() * s);
    int height = (int)(surface.get_height() * s);
    surface.put_screen((int)((pos.x + ofx) * s) - width/2, (int)((pos.y + ofy) * s) - height/2,
		       width, height);
  }
}

bool
Particle::is_alive()
{
  return livetime;
}

/* EOF */
