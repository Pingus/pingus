//  $Id: bridger.cc,v 1.28 2000/10/18 20:16:36 grumbel Exp $
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
#include "../PingusSound.hh"
#include "../algo.hh"
#include "bridger.hh"

using namespace std;

CL_Surface* Bridger::brick_l = 0;
CL_Surface* Bridger::brick_r = 0;
CL_Surface* Bridger::static_surface = 0;

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

  if (!static_surface)
    static_surface = CL_Surface::load("Pingus/bridger", local_res());
  surface = static_surface;

  if (!brick_l)
    brick_l = CL_Surface::load("Other/brick_left", local_res());
  if (!brick_r)
    brick_r = CL_Surface::load("Other/brick_right", local_res());

  bricks = 15;

  counter.set_size(surface->get_num_frames()/2);
  counter.set_speed(1);
  is_multi_direct = true;
  step = 0;
  do_steps = 0;
}

void
Bridger::draw_offset(int x, int y, float s)
{
  if (s == 1.0) 
    {
      surface->put_screen(pingu->x_pos + x + x_offset(), pingu->y_pos + y + y_offset(),
			  do_steps + ((pingu->direction.is_left()) ? 0 : counter.get_size()));
    }
  else 
    {
      surface->put_screen(int((pingu->x_pos + x + x_offset()) * s), int((pingu->y_pos + y + y_offset() - 1) * s), 
			  s, s, do_steps + ((pingu->direction.is_left()) ? 0 : counter.get_size()));
    }
}

void
Bridger::let_move()
{
  // Increment the animation only every first and second loop, not at the third
  if (step > 0) {
    ++do_steps;
    step = 0;
  }
  ++step;

  if (do_steps > 14)
    do_steps = 0;
  
  if (do_steps == 8)
    {
      if (bricks > 0)
	{
	  if (way_is_free())
	    {
	      place_a_brick();
	      walk_one_step_up();
	    }
	  else // We reached a wall...
	    {
	      pingu->direction.change();
	      is_finished = true;
	    }
	}
      else
	{
	  pingu->set_action(pingu->get_world()->get_action_holder()->get_uaction("waiter"));
	}
    }

  //std::cout << "bricks: " << bricks << std::endl;
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
      //cout << "Touched a wall" << endl;
      return false;
    }

  if (rel_getpixel(2, 26) == ColMap::NOTHING)
    {
      ret_val = true;
    }
  else
    {
      //cout << "Ouch, my head" << endl;
      return false;
    }

  return ret_val;
}

void
Bridger::place_a_brick()
{
  bricks--; 

  if (bricks < 4)
    PingusSound::play_wav("ting");
 
  if (pingu->direction.is_right())
    {
      pingu->get_world()->get_colmap()->put(brick_r, 
					    pingu->x_pos + 10 - brick_r->get_width(),
					    pingu->y_pos,
					    GroundpieceData::BRIDGE);
      pingu->get_world()->get_gfx_map()->put(brick_r->get_provider(), 
					     pingu->x_pos + 10 - brick_r->get_width(),
					     pingu->y_pos);
    }
  else
    {
      pingu->get_world()->get_colmap()->put(brick_r, pingu->x_pos - 10,
					    pingu->y_pos,
					    GroundpieceData::BRIDGE);
      pingu->get_world()->get_gfx_map()->put(brick_l->get_provider(), 
					     pingu->x_pos - 10,
					     pingu->y_pos);
    }
}

void
Bridger::walk_one_step_up()
{
  pingu->x_pos += 4 * pingu->direction;
  pingu->y_pos -= 2;
  counter = 0;
}

int
Bridger::x_offset(void)
{
  return -16;
}

int
Bridger::y_offset(void)
{
  return -29;
}

/* EOF */
