//  $Id: Spike.cc,v 1.4 2000/07/30 01:47:38 grumbel Exp $
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

#include "../PinguEnums.hh"
#include "../PingusResource.hh"

#include "Spike.hh"

Spike::Spike(TrapData data)
{
  killing = false;
  pos = data.pos;

  surface = PingusResource::load_surface("Traps/spike", "traps");

  counter.set_size(surface->get_num_frames());
  counter.set_type(GameCounter::once);
  counter.set_speed(1);
  counter = 0;
}

Spike::~Spike()
{

}
  
void
Spike::draw_offset(int x, int y, float s)
{
  if (killing) {
    surface->put_screen(pos.x_pos + x, pos.y_pos + y, counter);
  } else {
    // do nothing
  }
}

void
Spike::let_move(void)
{
  if (killing)
    ++counter;
  
  if (counter == (int)(surface->get_num_frames()) - 1) {
    killing = false;
    counter = 0;
  }
}

bool
Spike::catch_pingu(Pingu* pingu)
{
  if (!killing) {
    if (pingu->x_pos > pos.x_pos +16 - 5 && pingu->x_pos < pos.x_pos + 16 + 5
	&& pingu->y_pos > pos.y_pos && pingu->y_pos < pos.y_pos + 32) 
      {
	counter = 0;
	killing = true;
	return true;
      }
  } else {
    if (counter == 3 && pingu->x_pos > pos.x_pos +16 - 12 && pingu->x_pos < pos.x_pos + 16 + 12
	&& pingu->y_pos > pos.y_pos && pingu->y_pos < pos.y_pos + 32) 
      {
	pingu->set_status((PinguStatus)dead);
	return true;
      }
  }  
  return false;
}

/* EOF */
