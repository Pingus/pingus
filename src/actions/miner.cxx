// $Id: miner.cxx,v 1.16 2002/10/13 20:25:00 torangan Exp $
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

#include "../pingu_map.hxx"
#include "../col_map.hxx"
#include "../graphic_context.hxx"
#include "../world.hxx"
#include "../pingus_resource.hxx"
#include "../sound.hxx"
#include "../pingu.hxx"
#include "miner.hxx"

namespace Actions {

Miner::Miner (Pingu* p)
  : PinguAction(p),
    miner_radius(PingusResource::load_surface("Other/bash_radius", "pingus")),
    miner_radius_gfx(PingusResource::load_surface("Other/bash_radius_gfx", "pingus")),
    sprite(Sprite("Pingus/miner0", "pingus", 20.0f)),
    slow_count(0)
{
  sprite.set_align_center_bottom();
}

void
Miner::update ()
{
  sprite.update();

  // FIXME: Direction handling is ugly
  if (pingu->direction.is_left())
    sprite.set_direction(Sprite::LEFT);
  else
    sprite.set_direction(Sprite::RIGHT);

  ++slow_count;
  if (!(slow_count % 4))
    {
      if (!(slow_count % 3))
	{
	  WorldObj::get_world()->get_colmap()->remove(miner_radius.get_provider(), 
						      static_cast<int>(pingu->get_x() - 16 + pingu->direction), 
						      static_cast<int>(pingu->get_y() - 31));
	  WorldObj::get_world()->get_gfx_map()->remove(miner_radius_gfx.get_provider(), 
						       static_cast<int>(pingu->get_x() - 16 + pingu->direction),
						       static_cast<int>(pingu->get_y() - 31));
	}

      pingu->set_pos(pingu->get_x() + pingu->direction, pingu->get_y() + 1);
    }

  if (rel_getpixel(0, -1) == Groundtype::GP_NOTHING)
    {
      WorldObj::get_world()->get_colmap()->remove(miner_radius, 
					          static_cast<int>(pingu->get_x() - 16 + pingu->direction),
					          static_cast<int>(pingu->get_y () - 29));
      WorldObj::get_world()->get_gfx_map()->remove(miner_radius_gfx,
						   static_cast<int>(pingu->get_x() - 16 + pingu->direction),
						   static_cast<int>(pingu->get_y() - 29));
      pingu->set_action(Actions::Walker);
    }
  else if (rel_getpixel(0, -1) == Groundtype::GP_SOLID)
    {
      PingusSound::play_sound("sounds/chink.wav");
      WorldObj::get_world()->get_colmap ()->remove(miner_radius,
	                                           static_cast<int>(pingu->get_x() - 16 + pingu->direction),
					           static_cast<int>(pingu->get_y() - 31));
      WorldObj::get_world()->get_gfx_map()->remove(miner_radius_gfx,
	                                           static_cast<int>(pingu->get_x() - 16 + pingu->direction),
						   static_cast<int>(pingu->get_y() - 31));
      pingu->set_action(Actions::Walker);
    }
}

void 
Miner::draw (GraphicContext& gc)
{
  gc.draw(sprite, pingu->get_pos());
}

} // namespace Actions

/* EOF */
