//  $Id: laser_exit.cxx,v 1.3 2002/06/26 17:43:18 grumbel Exp $
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

#include "../world.hxx"
#include "../pingus_resource.hxx"
#include "../pingu_action_factory.hxx"
#include "../pingu_holder.hxx"
#include "laser_exit.hxx"
#include "../pingu.hxx"
#include "../pingu_action.hxx"

LaserExit::LaserExit(const TrapData& data)
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
LaserExit::update(float /*delta*/)
{

  PinguHolder* holder = world->get_pingu_p ();
  for (PinguIter pingu = holder->begin (); pingu != holder->end (); ++pingu){
    catch_pingu(*pingu);
  }

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

  if (!killing) 
    {
      if (pingu->get_x () < pos.x + 34 + 10 && pingu->get_x () > pos.x + 34 
	  && pingu->get_y () < pos.y + 43 + 20 && pingu->get_y () > pos.y + 43) 
	{
	  if (!(pingu->get_action() && pingu->get_action()->get_name() == "LaserKill")) 
	    {
	      killing = true;
	      pingu->set_action("laserkill");
	    }
	}
    }
}

/* EOF */
