//  $Id: LaserExit.cc,v 1.10 2001/03/31 10:54:27 grumbel Exp $
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

#include "../World.hh"
#include "../ActionHolder.hh"
#include "../PingusResource.hh"
#include "LaserExit.hh"

LaserExit::LaserExit(TrapData data)
{
  killing = false;
  pos = data.pos;

  surface = PingusResource::load_surface("Traps/laser_exit", "traps");

  counter.set_size(surface.get_num_frames());
  counter.set_type(GameCounter::once);
  counter.set_speed(5);
  counter = 0;

}

LaserExit::~LaserExit()
{
  
}

void
LaserExit::let_move(void)
{
  if (killing) {
    if (counter.finished()) {
      counter = 0;
      killing = 0;
    } else {
      ++counter;
    }
  }
}

void
LaserExit::catch_pingu(Pingu* pingu)
{
  //if (!pingu->is_alive())
  //return;

  if (!killing) {
    if (pingu->get_x () < pos.x_pos + 34 + 10 && pingu->get_x () > pos.x_pos + 34 
	&& pingu->get_y () < pos.y_pos + 43 + 20 && pingu->get_y () > pos.y_pos + 43) 
      {
	if (!(pingu->get_action().get() && pingu->get_action()->name() == "LaserKill")) {
	  killing = true;
	  pingu->set_action(world->get_action_holder()->get_uaction("laserkill"));
	}
      }
  }
}

/* EOF */
