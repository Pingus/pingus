//  $Id: Entrance.cc,v 1.10 2000/06/19 20:10:38 grumbel Exp $
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

Entrance::Entrance(entrance_data data)
{
  if (verbose > 2)
    std::cout << "Creating Entrance" << std::endl;
  x_pos = data.x_pos;
  y_pos = data.y_pos;
  release_rate = data.release_rate;
  last_release = -release_rate;
  z_pos = -10;
  direction = data.direction;

  surface = 0;//CL_Surface::load("Entrances/entrance", PingusResource::get("global.dat"));
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
  p = new Pingu(x_pos, y_pos);
  
  switch (direction) {

  case entrance_data::LEFT:
    d.left();
    p->set_direction(d);
    break;

  case entrance_data::MISC:
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
	
  case entrance_data::RIGHT:  
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
      surface->put_screen(x_pos - 32 + x, y_pos - 16 + y);
    } 
  else 
    {
      surface->put_screen((int)((x_pos-32 + x) * s), (int)((y_pos-16 + y) * s),
			  s, s);
    }
}

void
Entrance::set_action_holder(ActionHolder* a)
{
  action_holder = a;
}

/* EOF */
