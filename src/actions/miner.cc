// $Id: miner.cc,v 1.30 2002/06/01 18:05:36 torangan Exp $
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

#include "../PinguMap.hh"
#include "../ColMap.hh"
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
  miner_radius = PingusResource::load_surface ("Other/bash_radius", "pingus");
  miner_radius_gfx = PingusResource::load_surface ("Other/bash_radius_gfx", "pingus");
  sprite = Sprite ("Pingus/miner0", "pingus", 20.0f);
  sprite.set_align_center_bottom ();
  slow_count = 0;
}

void
Miner::update(float delta)
{
  sprite.update (delta);
  
  // FIXME: Direction handling is ugly
  if (pingu->direction.is_left ())
    sprite.set_direction (Sprite::LEFT);
  else
    sprite.set_direction (Sprite::RIGHT);

  ++slow_count;
  if (slow_count % 4  == 0) 
    {
      if (slow_count % 3 == 0) 
	{
	  pingu->get_world()->get_colmap()->remove(miner_radius.get_provider(), 
						   pingu->get_x () - 16 + pingu->direction, 
						   pingu->get_y () - 31);
	  pingu->get_world()->get_gfx_map()->remove(miner_radius_gfx.get_provider(), 
						    pingu->get_x () - 16 + pingu->direction, 
						    pingu->get_y () - 31);
	}

      pingu->pos.x += pingu->direction;
      pingu->pos.y += 1;
    }
  
  if (rel_getpixel(0, -1) == ColMap::NOTHING)
    {
      pingu->get_world()->get_colmap()->remove(miner_radius, 
					       pingu->get_x () - 16 + pingu->direction, 
					       pingu->get_y () - 29);
      pingu->get_world()->get_gfx_map()->remove(miner_radius_gfx,
						pingu->get_x () - 16 + pingu->direction, 
						pingu->get_y () - 29);
      is_finished = true;
    }
  else if (rel_getpixel(0, -1) & ColMap::SOLID)
    {
      PingusSound::play_sound("sounds/chink.wav");
      pingu->get_world()->get_colmap()->remove(miner_radius, pingu->get_x () - 16 + pingu->direction, 
					       pingu->get_y () - 31);
      pingu->get_world()->get_gfx_map()->remove(miner_radius_gfx, pingu->get_x () - 16 + pingu->direction, 
						pingu->get_y () - 31);
      is_finished = true;
    }
}

void 
Miner::draw_offset(int x, int y, float /*s*/)
{
  sprite.put_screen (pingu->get_pos () + CL_Vector(x, y));
}

/* EOF */
