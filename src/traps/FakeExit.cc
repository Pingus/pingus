//  $Id: FakeExit.cc,v 1.17 2001/12/16 03:23:44 cagri Exp $
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
#include "FakeExit.hh"
#include "../PingusResource.hh"
#include "../PinguActionFactory.hh"
#include "../PinguHolder.hh"

FakeExit::FakeExit(const TrapData& data)
{
  surface = PingusResource::load_surface("Traps/fake_exit", "traps");
  pos = data.pos;
  
  counter.set_size(surface.get_num_frames());
  counter.set_type(GameCounter::once);
  counter.set_speed(2.5);
  counter = surface.get_num_frames() - 1;
  smashing = false;
}

FakeExit::~FakeExit()
{
  
}

void
FakeExit::update(float delta)
{
  PinguHolder* holder = world->get_pingu_p ();
  for (PinguIter pingu = holder->begin (); pingu != holder->end (); ++pingu){
       catch_pingu(*pingu);
  }

  if (smashing)
    ++counter;
}

void
FakeExit::catch_pingu(boost::shared_ptr<Pingu> pingu)
{
  //  if (!pingu->is_alive())
  //  return false;
  
  if (counter.finished()) {
    smashing = false;
  }
  
  if (pingu->get_x() > pos.x + 31 && pingu->get_x() < pos.x + 31 + 15
      && pingu->get_y() > pos.y + 56 && pingu->get_y() < pos.y + 56+56) 
    {
      if (!smashing) {
	counter = 0;
	smashing = true; 
      }

      if (counter >= 3 && counter <= 5) {
	pingu->set_action(PinguActionFactory::instance ()->create_sp ("smashed"));
      }
    }
}

/* EOF */
