//  $Id: basher.cc,v 1.4 2000/03/01 21:12:44 grumbel Exp $
//
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

#include "basher.hh"
#include "../particles/GroundParticle.hh"
#include "../algo.hh"

Basher::Basher()
{
}

PinguAction* 
Basher::allocate(void)
{
  return new Basher();
}

void
Basher::init(void)
{
  action_name = "Basher";
  environment = (PinguEnvironment)land;

  surface = CL_Surface::load("Pingus/basher", local_res());
  bash_radius = CL_Surface::load("Other/bash_radius", local_res());
  
  counter.set_size(surface->get_num_frames() / 2);
  counter.set_type(GameCounter::loop);
  //counter.set_count(0);
  counter.set_speed(1);

  basher_c.set_size(2);
  basher_c.set_speed(1);

  is_multi_direct = true;
  first_bash = true;
}

void
Basher::let_move()
{
  ++counter;
  walk_forward();

  if (basher_c++ == 0)
    {
      if (have_something_to_dig())
	{
	  bash();
	}
      else
	{
	  is_finished = true;
	}
    }
}

void
Basher::bash()
{
  pingu->colmap->remove(bash_radius->get_provider(), 
			pingu->x_pos - (bash_radius->get_width()/2), pingu->y_pos - 32);
  pingu->map->remove(bash_radius->get_provider(), 
		     pingu->x_pos - (bash_radius->get_width()/2), pingu->y_pos - 32);
}

void
Basher::walk_forward()
{
  pingu->x_pos += pingu->direction;
}

bool
Basher::have_something_to_dig()
{
  if (first_bash)
    {
      first_bash = false;
      return true;
    }

  for(int i = 0; is_finished == false && i < 16; i++)
    {
      cout << rel_getpixel(i,0) << " " << flush;
      if (rel_getpixel(i,0) & ColMap::WALL)
	{
	  cout << "Found something to dig..." << endl;
	  return true;
	}
    }

  cout << "nothing to dig found" << endl;
  return false;
}

/* EOF */
