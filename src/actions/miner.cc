// $Id: miner.cc,v 1.13 2000/12/14 21:35:55 grumbel Exp $
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

#include "../PingusSound.hh"
#include "../algo.hh"
#include "miner.hh"

Miner::Miner()
{
}

PinguAction* 
Miner::allocate(void)
{
  return new Miner();
}

void
Miner::init(void)
{
  environment = (PinguEnvironment)land;
  action_name = "Miner";

  miner_radius = CL_Surface ("Other/bash_radius", local_res());
  surface = CL_Surface ("Pingus/miner", local_res());
  counter.set_size(surface.get_num_frames()/2);
  counter.set_speed(10);
  is_multi_direct = true;
  //  slow_count.set_size(3);
  slow_count = 0;
}

void
Miner::let_move()
{
  ++slow_count;
  if (slow_count % 4  == 0) 
    {
      if (slow_count % 3 == 0) 
	{
	  pingu->get_world()->get_colmap()->remove(miner_radius.get_provider(), pingu->x_pos - 16 + pingu->direction, 
						   pingu->y_pos - 31);
	  pingu->get_world()->get_gfx_map()->remove(miner_radius.get_provider(), pingu->x_pos - 16 + pingu->direction, 
						    pingu->y_pos - 31);
	}

      pingu->x_pos += pingu->direction;
      pingu->y_pos += 1;
    }
  
  if (rel_getpixel(0, -1) == ColMap::NOTHING)
    {
      pingu->get_world()->get_colmap()->remove(miner_radius, pingu->x_pos - 16 + pingu->direction, pingu->y_pos - 29);
      pingu->get_world()->get_gfx_map()->remove(miner_radius, pingu->x_pos - 16 + pingu->direction, pingu->y_pos - 29);
      is_finished = true;
    }
  else if (rel_getpixel(0, -1) & ColMap::SOLID)
    {
      PingusSound::play_wav("chink");
      pingu->get_world()->get_colmap()->remove(miner_radius, pingu->x_pos - 16 + pingu->direction, 
					       pingu->y_pos - 31);
      pingu->get_world()->get_gfx_map()->remove(miner_radius, pingu->x_pos - 16 + pingu->direction, 
						pingu->y_pos - 31);
      is_finished = true;
    }
}

/* EOF */
