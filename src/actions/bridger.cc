//  $Id: bridger.cc,v 1.7 2000/03/10 18:56:13 grumbel Exp $
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
  waiter  = CL_Surface::load("Pingus/blocker", local_res());
  brick_r = CL_Surface::load("Other/brick_left", local_res());
  brick_l = CL_Surface::load("Other/brick_right", local_res());
  bricks = 30;

  counter.set_size(surface->get_num_frames()/2);
  counter.set_speed(1);
  is_multi_direct = true;
  step = 0;
  do_steps = 0;

  // Ressetting the bridger position a bit, so that we don't get a
  // hole at the start of the bridge
  pingu->x_pos -= pingu->direction * 2;
} 

void
Bridger::draw_offset(int x, int y, float s)
{
  if (s == 1.0) 
    {
      if (bricks > 0)
	{
	  surface->put_screen(pingu->x_pos + x + x_offset(), pingu->y_pos + y + y_offset() - 1, 
			      ++counter + ((pingu->direction.is_left()) ? 0 : counter.get_size()));
	}
      else
	{
	  waiter->put_screen(pingu->x_pos + x + x_offset(), 
			     pingu->y_pos + y + y_offset());
	}
    } 
  else 
    {
      surface->put_screen(int((pingu->x_pos + x + x_offset()) * s), int((pingu->y_pos + y + y_offset() - 1) * s), 
			  s, s, ++counter + ((pingu->direction.is_left()) ? 0 : counter.get_size()));
    }
}

void
Bridger::let_move()
{
  ++do_steps;
  
  if (do_steps > 11) 
    {
      do_steps = 0;

      if (bricks <= -2)
	{
	  is_finished = true;
	}
      else if (bricks > 0)
	{
	  if (way_is_free())
	    {
	      place_a_brick();
	      walk_one_step_up();
	    }
	  else
	    {
	      is_finished = true;
	    }
	}
      else
	{
	  // Waiting some seconds after we are out of bricks 
	  bricks--;
	}
    }
}

bool
Bridger::way_is_free()
{
  bool ret_val;
  
  if (rel_getpixel(2,2) == ColMap::NOTHING)
    {
      ret_val = true;
    }
  else
    {
      cout << "Touched a wall" << endl;
      return false;
    }

  if (rel_getpixel(2, 26) == ColMap::NOTHING)
    {
      ret_val = true;
    }
  else
    {
      cout << "Ouch, my head" << endl;
      return false;
    }

  return ret_val;
}

void
Bridger::place_a_brick()
{
  bricks--; 
  
  if (pingu->direction.is_right())
    {
      pingu->colmap->put(brick_r, 
			 pingu->x_pos + 16 - brick_r->get_width(),
			 pingu->y_pos - 1,
			 surface_data::BRIDGE);
      pingu->map->put(brick_r->get_provider(), 
		      pingu->x_pos + 16 - brick_r->get_width(),
		      pingu->y_pos - 1);
    }
  else
    {
      pingu->colmap->put(brick_r, pingu->x_pos - 16,
			 pingu->y_pos - 1,
			 surface_data::BRIDGE);
      pingu->map->put(brick_l->get_provider(), 
		      pingu->x_pos -16,
		      pingu->y_pos - 1);
    }
}

void
Bridger::walk_one_step_up()
{
  pingu->x_pos += 2 * pingu->direction;
  ++step;
  
  if (step >= 2) 
    {
      pingu->y_pos -= 2;
      step = 0;
    }

  counter = 0;
}

/* EOF */
