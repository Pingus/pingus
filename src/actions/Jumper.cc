//  $Id: Jumper.cc,v 1.7 2000/12/14 21:35:55 grumbel Exp $
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

#include "Jumper.hh"

bool Jumper::static_surfaces_loaded;
CL_Surface Jumper::static_surface;

Jumper::Jumper()
{
  // do nothing
}

PinguAction* 
Jumper::allocate(void)
{
  return new Jumper;
}

void
Jumper::init(void)
{
  action_name = "Jumper";

  if (!static_surfaces_loaded)
    static_surface = CL_Surface ("Pingus/jumper", local_res());
  surface = static_surface;

  environment = (PinguEnvironment)(land);
  counter.set_size(4);
  counter.set_type(Counter::loop);
}

void
Jumper::let_move()
{
  if (pingu->direction.is_right()) {
    pingu->velocity += CL_Vector(10.0, -10.0);
  } else {
    pingu->velocity += CL_Vector(-10.0, -10.0);
  }

  pingu->y_pos -= 1;

  is_finished = true;
}

/* EOF */
