//  $Id: bridger.cc,v 1.1 2000/02/04 23:45:19 mbn Exp $
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
#include "bridger.hh"

Bridger::Bridger()
{
}

PinguAction*
Bridger::allocate(void)
{
  return new Bridger();
}

void
Bridger::init(void)
{
  environment = (PinguEnvironment)land;
  action_name = "Bridger";

  surface = CL_Surface::load("Pingus/bridger", local_res());
  brick   = CL_Surface::load("Other/brick", local_res());
  bricks = 30;

  counter.set_size(surface->get_num_frames()/2);
  counter.set_speed(10);
  is_multi_direct = true;
  step = 0;
  do_steps = 0;
}

void
Bridger::let_move()
{
  ++do_steps;
  
  if (do_steps > 2) 
    {
      do_steps = 0;
      if (bricks > 0) 
	{
	  if((rel_getpixel(2,2) == ColMap::NOTHING) || (bricks > 30))
	    {
	      pingu->colmap->put(brick, pingu->x_pos, pingu->y_pos, surface_data::BRIDGE);
	      pingu->map->put(brick->get_provider(), pingu->x_pos, pingu->y_pos);
	      --bricks;

	      pingu->x_pos += 2 * pingu->direction;

	      ++step;
	      if (step >= 1) {
		pingu->y_pos -= 1;
		step = 0;
	      }	  
	    } 
	  else 
	    {
	      if (rel_getpixel(1,1) == ColMap::NOTHING) 
		{
		  pingu->colmap->put(brick,pingu->x_pos,pingu->y_pos, surface_data::BRIDGE);
		  pingu->map->put(brick->get_provider(), pingu->x_pos, pingu->y_pos);
		  --bricks;
	      
		  pingu->x_pos += pingu->direction;
	      
		  ++step;
		  if (step >= 1) {
		    pingu->y_pos -= 1;
		    step = 0;
		  }
		}
	      is_finished = true;
	    }
	}
      else 
	{
	  is_finished = true;
	}
    }
}

/* EOF */
