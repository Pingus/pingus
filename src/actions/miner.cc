// $Id: miner.cc,v 1.17 2001/04/08 14:10:34 grumbel Exp $
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
#include "../PingusResource.hh"
#include "../PingusSound.hh"
#include "../algo.hh"
#include "miner.hh"

Miner::Miner()
{
}

void
Miner::init(void)
{
  environment = (PinguEnvironment)land;
  action_name = "Miner";

  miner_radius = PingusResource::load_surface ("Other/bash_radius", "pingus");
  surface = PingusResource::load_surface ("Pingus/miner", "pingus");
  counter.set_size(surface.get_num_frames()/2);
  counter.set_speed(10);
  is_multi_direct = true;
  //  slow_count.set_size(3);
  slow_count = 0;
}

void
Miner::update(float delta)
{
  ++slow_count;
  if (slow_count % 4  == 0) 
    {
      if (slow_count % 3 == 0) 
	{
	  pingu->get_world()->get_colmap()->remove(miner_radius.get_provider(), pingu->get_x () - 16 + pingu->direction, 
						   pingu->get_y () - 31);
	  pingu->get_world()->get_gfx_map()->remove(miner_radius.get_provider(), pingu->get_x () - 16 + pingu->direction, 
						    pingu->get_y () - 31);
	}

      pingu->pos.x += pingu->direction;
      pingu->pos.y += 1;
    }
  
  if (rel_getpixel(0, -1) == ColMap::NOTHING)
    {
      pingu->get_world()->get_colmap()->remove(miner_radius, pingu->get_x () - 16 + pingu->direction, pingu->get_y () - 29);
      pingu->get_world()->get_gfx_map()->remove(miner_radius, pingu->get_x () - 16 + pingu->direction, pingu->get_y () - 29);
      is_finished = true;
    }
  else if (rel_getpixel(0, -1) & ColMap::SOLID)
    {
      PingusSound::play_wav("chink");
      pingu->get_world()->get_colmap()->remove(miner_radius, pingu->get_x () - 16 + pingu->direction, 
					       pingu->get_y () - 31);
      pingu->get_world()->get_gfx_map()->remove(miner_radius, pingu->get_x () - 16 + pingu->direction, 
						pingu->get_y () - 31);
      is_finished = true;
    }
}

/* EOF */
