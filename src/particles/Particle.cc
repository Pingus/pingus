//  $Id: Particle.cc,v 1.7 2001/04/03 10:45:50 grumbel Exp $
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
{
  x_pos = x;
  y_pos = y;
  x_add = x_a;
  y_add = y_a;
  livetime = 50 + ( rand() % 25);
}

Particle::~Particle()
{
  
}

void
Particle::init(int x, int y, float x_a, float y_a)
{
  x_pos = x;
  y_pos = y;
  x_add = x_a;
  y_add = y_a;
  livetime = 50 + ( rand() % 25);
}

void
Particle::update(float delta)
{
  x_pos += x_add;
  y_pos += y_add;
  y_add += 0.1;

  if (livetime > 0)
    --livetime;
}

void
Particle::draw_offset(int ofx, int ofy, float s)
{
  if (s == 1.0) {
    surface.put_screen(x_pos + ofx, y_pos + ofy);
  } else {
    int width  = (int)(surface.get_width() * s);
    int height = (int)(surface.get_height() * s);
    surface.put_screen((int)((x_pos + ofx) * s) - width/2, (int)((y_pos + ofy) * s) - height/2,
		       width, height);
  }
}

bool
Particle::is_alive()
{
  return livetime;
}

/* EOF */
