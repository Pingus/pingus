//  $Id: Guillotine.cc,v 1.15 2001/12/16 03:23:44 cagri Exp $
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
#include "Guillotine.hh"
#include "../PinguHolder.hh"
#include "../World.hh"

Guillotine::Guillotine(const TrapData& data)
{
  killing = false;
  pos = data.pos;
  
  surface = PingusResource::load_surface("Traps/guillotinekill", "traps");
  idle_surf = PingusResource::load_surface("Traps/guillotineidle", "traps");

  counter.set_size(surface.get_num_frames()/2);
  counter.set_type(GameCounter::once);
  counter.set_speed(0.7);
  counter = 0;

  idle_counter.set_size(idle_surf.get_num_frames());
  idle_counter.set_type(GameCounter::loop);
  idle_counter.set_speed(0.3);
  idle_counter = 0;
}

Guillotine::~Guillotine()
{
}

void
Guillotine::draw_offset(int x, int y, float s)
{
  if (killing) {
    if (direction.is_left())
      surface.put_screen(int(pos.x + x), int(pos.y + y), counter);
    else
      surface.put_screen(int(pos.x + x), int(pos.y + y), counter + 12);
  } else {
    idle_surf.put_screen(int(pos.x + x), int(pos.y + y), idle_counter);
  }
}

void
Guillotine::update(float delta)
{
  if (counter.finished()) {
    counter = 0;
    killing = false;
  }

  PinguHolder* holder = world->get_pingu_p ();
  for (PinguIter pingu = holder->begin (); pingu != holder->end (); ++pingu) {
	  catch_pingu(*pingu);
  }

  if (killing) {
    ++counter;
  } else {
    ++idle_counter;
  }
}

void
Guillotine::catch_pingu(boost::shared_ptr<Pingu> pingu)
{
  if (!killing) 
    {
      if (pingu->is_inside (int(pos.x + 38), int(pos.y + 90),
			    int(pos.x + 42), int(pos.y + 98)))
	{
	  killing = true;
	  pingu->set_status(PS_DEAD);
	  direction = pingu->direction;
	}
    }
}

/* EOF */
