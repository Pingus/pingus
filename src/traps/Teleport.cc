//  $Id: Teleport.cc,v 1.6 2000/09/18 12:22:16 grumbel Exp $
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

#include "Teleport.hh"
#include "../PingusResource.hh"
#include "../actions/teleported.hh"

// FIXME: Just for the record, I don't really like to have x_target
// and y_target in Trap, there should probably some other way of
// handling this.

Teleport::Teleport(trap_data data)
{
  surface = PingusResource::load_surface("Traps/teleporter", "traps");
  pos = data.pos;

  x_target = data.x_target;
  y_target = data.y_target;

  counter.set_size(surface->get_num_frames());
  counter.set_type(GameCounter::once);
  counter.set_speed(1);
  counter = surface->get_num_frames() - 1;
  teleporting = false;
}

Teleport::~Teleport()
{
  
}

void
Teleport::let_move()
{
  if (teleporting)
    ++counter;
}

void
Teleport::catch_pingu(Pingu* pingu)
{
  Teleported *teleported;

  //if (!pingu->is_alive())
  //return false;
  
  if (counter.finished()) {
    teleporting = false;
  }
  
  if (pingu->get_x() > x_pos  && pingu->get_x() < x_pos + 35
      && pingu->get_y() > y_pos && pingu->get_y() < y_pos + 52) 
    {
      if (!teleporting) {
	counter = 0;
	teleporting = true; 
      }

      if (counter >= 3 && counter <= 5) 
	{
	  teleported = new Teleported();
	  teleported->x_target = x_target;
	  teleported->y_target = y_target;
	  pingu->set_action(teleported);
	}
      return true;
    } else {
      return false;
    }
}

/* EOF */
