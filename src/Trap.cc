//  $Id: Trap.cc,v 1.6 2000/08/03 10:31:17 grumbel Exp $
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

#include "Trap.hh"

Trap::Trap()
{
  //z_pos = 200;
}

Trap::~Trap()
{
  
}

void 
Trap::draw_offset(int x, int y, float s)
{
  assert(surface);
  //cout << "Count: " << counter.value() << std::endl;
  if (s == 1.0) {
    surface->put_screen(pos.x_pos + x, pos.y_pos + y,
			counter.value());
  } else {
    surface->put_screen((int)((pos.x_pos + x) * s), 
			(int)((pos.y_pos + y) * s),
			s, s, counter.value());
  }
}

void 
Trap::let_move()
{
  // do nothing
}

bool 
Trap::catch_pingu(Pingu* pingu)
{
  return false;
}

/* EOF */
