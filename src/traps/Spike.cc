//  $Id: Spike.cc,v 1.15 2002/06/01 18:05:37 torangan Exp $
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
#include "../PinguHolder.hh"
#include "../World.hh"

#include "Spike.hh"

Spike::Spike(const TrapData& data)
{
  killing = false;
  pos = data.pos;

  surface = PingusResource::load_surface("Traps/spike", "traps");

  counter.set_size(surface.get_num_frames());
  counter.set_type(GameCounter::once);
  counter.set_speed(1);
  counter = 0;
}

Spike::~Spike()
{

}
  
void
Spike::draw_offset(int x, int y, float /*s*/)
{
  if (killing) {
    surface.put_screen(int(pos.x + x), int(pos.y + y), counter);
  } else {
    // do nothing
  }
}

void
Spike::update(float /*delta*/)
{
  if (killing)
    ++counter;
  
  PinguHolder* holder = world->get_pingu_p ();
  for (PinguIter pingu = holder->begin (); pingu != holder->end (); ++pingu){
       catch_pingu(*pingu);
  }

  if (counter == (int)(surface.get_num_frames()) - 1) {
    killing = false;
    counter = 0;
  }
}

void
Spike::catch_pingu(boost::shared_ptr<Pingu> pingu)
{
  if (!killing) {
    if (pingu->get_x () > pos.x + 16 - 5 && pingu->get_x () < pos.x + 16 + 5
	&& pingu->get_y () > pos.y && pingu->get_y () < pos.y + 32) 
      {
	counter = 0;
	killing = true;
      }
  } else {
    if (counter == 3 && pingu->get_x () > pos.x +16 - 12 && pingu->get_x () < pos.x + 16 + 12
	&& pingu->get_y () > pos.y && pingu->get_y () < pos.y + 32) 
      {
	pingu->set_status(PS_DEAD);
      }
  }  
}

/* EOF */
