//  $Id: Teleport.cc,v 1.1 2000/02/04 23:45:19 mbn Exp $
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

Teleport::Teleport(trap_data data)
{
  surface = CL_Surface::load("Traps/teleporter", PingusResource::get("traps.dat"));
  x_pos = data.x_pos;
  y_pos = data.y_pos;
  z_pos = data.z_pos;
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

bool
Teleport::catch_pingu(Pingu* pingu)
{
  Teleported *teleported;

  if (!pingu->is_alive())
    return false;
  
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

      if (counter >= 3 && counter <= 5) {
	teleported = new Teleported();
	pingu->set_action(teleported, -1);
	teleported->x_target = x_target;
	teleported->y_target = y_target;
      }
      return true;
    } else {
      return false;
    }
}

/* EOF */
