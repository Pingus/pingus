//  $Id: FakeExit.cc,v 1.4 2000/06/06 18:51:52 grumbel Exp $
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

#include "FakeExit.hh"
#include "../PingusResource.hh"
#include "../ActionHolder.hh"

FakeExit::FakeExit(trap_data data)
{
  surface = CL_Surface::load("Traps/fake_exit", PingusResource::get("traps.dat"));
  x_pos = data.x_pos;
  y_pos = data.y_pos;
  z_pos = data.z_pos;
  
  counter.set_size(surface->get_num_frames());
  counter.set_type(GameCounter::once);
  counter.set_speed(2.5);
  counter = surface->get_num_frames() - 1;
  smashing = false;
}

FakeExit::~FakeExit()
{
  
}

void
FakeExit::let_move()
{
  if (smashing)
    ++counter;
}

bool
FakeExit::catch_pingu(Pingu* pingu)
{
  if (!pingu->is_alive())
    return false;
  
  if (counter.finished()) {
    smashing = false;
  }
  
  if (pingu->get_x() > x_pos + 31 && pingu->get_x() < x_pos + 31 + 15
      && pingu->get_y() > y_pos + 56 && pingu->get_y() < y_pos + 56+56) 
    {
      if (!smashing) {
	counter = 0;
	smashing = true; 
      }

      if (counter >= 3 && counter <= 5) {
	pingu->set_action(action_holder->get_uaction("smashed"));
      }
      return true;
    } else {
      return false;
    }
}

/* EOF */
