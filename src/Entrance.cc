//  $Id: Entrance.cc,v 1.12 2000/07/30 01:47:35 grumbel Exp $
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

#include <cassert>

#include "PingusResource.hh"
#include "PingusSound.hh"
#include "algo.hh"
#include "globals.hh"
#include "Entrance.hh"
#include "GameTime.hh"

ParticleHolder* Entrance::particle;
ActionHolder*   Entrance::action_holder;

void
Entrance::SetParticleHolder(ParticleHolder* p)
{
  particle = p;
}

Entrance::Entrance()
{
}

Entrance::Entrance(EntranceData data)
{
  if (verbose > 2)
    std::cout << "Creating Entrance" << std::endl;
  pos = data.pos;
  release_rate = data.release_rate;
  last_release = -release_rate;
  direction = data.direction;

  surface = 0;//CL_Surface::load("Entrances/entrance", global");
}

bool
Entrance::pingu_ready()
{
  if (last_release < (GameTime::get_time() - release_rate)) {
    last_release = GameTime::get_time();
    return true;
  } else {
    return false;
  }
}

Pingu*
Entrance::get_pingu()
{
  static int last_direction;
  Pingu* p;
  Direction d;

  // FIXME: Evil, the allocated objects are destroyed in PinguHolder,
  // FIXME: but all allocation should be encapsulet in PinguHolder.
  p = new Pingu(pos.x_pos, pos.y_pos);
  
  switch (direction) 
    {
    case EntranceData::LEFT:
      d.left();
      p->set_direction(d);
      break;

    case EntranceData::MISC:
      if (last_direction) 
	{
	  d.left();
	  last_direction = 0;
	} 
      else
	{
	  d.right();
	  last_direction = 1;
	}
      p->set_direction(d);
      break;
	
    case EntranceData::RIGHT:  
      d.right();
      p->set_direction(d);
      break;
    
    default:
      std::cout << "Entrance:: Warning direction is wrong: " << direction << std::endl;
      d.right();
      p->set_direction(d);
      break;
    }

  return p;
}

void
Entrance::let_move(void)
{
}

void
Entrance::draw_offset(int x, int y, float s)
{
  if (!surface) return;
  if (s == 1.0) 
    {
      // FIXME: Why do we have still this hardcoded offsets?!
      surface->put_screen(pos.x_pos - 32 + x, pos.y_pos - 16 + y);
    } 
  else 
    {
      surface->put_screen((int)((pos.x_pos-32 + x) * s),
			  (int)((pos.y_pos-16 + y) * s),
			  s, s);
    }
}

void
Entrance::set_action_holder(ActionHolder* a)
{
  action_holder = a;
}

/* EOF */
