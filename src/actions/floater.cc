//  $Id: floater.cc,v 1.7 2001/03/31 10:54:27 grumbel Exp $
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

#include "../PingusResource.hh"
#include "floater.hh"

Floater::Floater()
{
  is_multi_direct = false;
}

void
Floater::init(void)
{
  falling_depth = 0;
  step = 0;
  environment = PinguEnvironment(sky | land);
  action_name = "floater";
  surface = PingusResource::load_surface ("Pingus/floater", "pingus");
  counter.set_size(surface.get_num_frames());
  counter.set_speed(20);
}

void
Floater::let_move()
{
  pingu->falling = 0;
  pingu->velocity = CL_Vector(0.0, 0.0);
  if (rel_getpixel(0, -1) == 0) {
    ++step;
    if (step > 0) {
      ++(pingu->pos.y);
      step = 0;
    }
  } else {
    is_finished = true;
  }
}

/* EOF */
