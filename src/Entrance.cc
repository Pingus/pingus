//  $Id: Entrance.cc,v 1.28 2002/06/08 16:08:16 grumbel Exp $
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

#include "World.hh"
#include "PinguHolder.hh"
#include "PingusResource.hh"
#include "PingusSound.hh"
#include "algo.hh"
#include "globals.hh"
#include "Entrance.hh"
#include "GameTime.hh"

Entrance::Entrance(const EntranceData& data)
{
  if (verbose > 2)
    std::cout << "Creating Entrance" << std::endl;
  pos = data.pos;
  release_rate = data.release_rate;
  last_release = -release_rate;
  direction = data.direction;
  owner_id = data.owner_id;
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
  Direction d;

  Pingu* p (world->get_pingu_p()->create_pingu (pos, owner_id));
  
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
Entrance::update(float /*delta*/)
{
  if (pingu_ready () 
      && (world->get_released_pingus() < world->get_allowed_pingus())
      && (! world->check_armageddon()))
    {
      world->get_pingu_p()->add (get_pingu ());
      world->inc_released_pingus();
    }
}

void
Entrance::draw_offset(int x, int y, float s)
{
  if (!surface) return;
  if (s == 1.0) 
    {
      // FIXME: Why do we still have these hardcoded offsets?!
      surface.put_screen((int)pos.x - 32 + x, (int)pos.y - 16 + y);
    } 
  else 
    {
      surface.put_screen((int)((pos.x-32 + x) * s),
			 (int)((pos.y-16 + y) * s),
			 s, s);
    }
}

/* EOF */
