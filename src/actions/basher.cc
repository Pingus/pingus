//  $Id: basher.cc,v 1.9 2000/06/10 07:56:59 grumbel Exp $
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

using namespace std;

CL_Surface* Basher::static_surface = 0;
CL_Surface* Basher::bash_radius = 0;

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

  if (!static_surface)
    static_surface = CL_Surface::load("Pingus/basher", local_res());
  surface = static_surface;

  if (!bash_radius)
    bash_radius = CL_Surface::load("Other/bash_radius", local_res());
  
  counter.set_size(surface->get_num_frames() / 2);
  counter.set_type(GameCounter::loop);
  //counter.set_count(0);
  counter.set_speed(1);

  basher_c.set_size(2);
  basher_c.set_speed(.3);

  is_multi_direct = true;
  first_bash = true;
}

void
Basher::draw_offset(int x, int y, float s)
{
  if (s == 1.0) 
    {
      surface->put_screen(pingu->x_pos + x + x_offset(), pingu->y_pos + y + y_offset(), 
			  counter + ((pingu->direction.is_left()) ? 0 : counter.get_size()));
    } 
  else 
    {
      surface->put_screen(int((pingu->x_pos + x + x_offset()) * s), int((pingu->y_pos + y + y_offset()) * s), 
			  s, s, counter + ((pingu->direction.is_left()) ? 0 : counter.get_size()));
    }
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
			pingu->x_pos - (bash_radius->get_width()/2), pingu->y_pos - 31);
  pingu->map->remove(bash_radius->get_provider(), 
		     pingu->x_pos - (bash_radius->get_width()/2), pingu->y_pos - 31);
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
      //cout << rel_getpixel(i,0) << " " << flush;
      if (rel_getpixel(i,0) & ColMap::WALL)
	{
	  //cout << "Found something to dig..." << endl;
	  return true;
	}
    }

  cout << "nothing to dig found" << endl;
  return false;
}

/* EOF */
