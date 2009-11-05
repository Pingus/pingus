// $Id: miner.cpp 3216 2007-09-22 01:47:03Z grumbel $
////  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "pingus/actions/miner.hpp"

#include "pingus/ground_map.hpp"
#include "engine/display/scene_context.hpp"
#include "pingus/world.hpp"
#include "engine/sound/sound.hpp"
#include "pingus/pingu.hpp"
#include "pingus/worldobj.hpp"

namespace Actions {

Miner::Miner (Pingu* p) :
  PinguAction(p),
  miner_radius("other/bash_radius_gfx", "other/bash_radius"),
  sprite(),
  slow_count(0)
{
  sprite.load(Direction::LEFT,  Sprite("pingus/player" + 
                                       pingu->get_owner_str() + "/miner/left"));
  sprite.load(Direction::RIGHT, Sprite("pingus/player" + 
                                       pingu->get_owner_str() + "/miner/right"));
}

void
Miner::update ()
{
  sprite[pingu->direction].update();

  ++slow_count;
  if (!(slow_count % 4))
  {
    if (!(slow_count % 3))
    {
      WorldObj::get_world()->remove(miner_radius,
                                    static_cast<int>(pingu->get_x() - static_cast<float>((miner_radius.get_width() / 2) + pingu->direction)),
                                    static_cast<int>(pingu->get_y() - static_cast<float>(miner_radius.get_width() + 1)));
    }

    pingu->set_pos(pingu->get_x() + static_cast<float>(pingu->direction),
                   pingu->get_y() + 1.0f);
  }

  if (rel_getpixel(0, -1) == Groundtype::GP_NOTHING)
  {
    WorldObj::get_world()->remove(miner_radius,
                                  static_cast<int>(pingu->get_x() - static_cast<float>((miner_radius.get_width() / 2) + pingu->direction)),
                                  static_cast<int>(pingu->get_y() - static_cast<float>(miner_radius.get_width() + 3) ));
    pingu->set_action(Actions::WALKER);
  }
  else if (rel_getpixel(0, -1) == Groundtype::GP_SOLID
           || rel_getpixel(0, pingu_height) == Groundtype::GP_SOLID)
  {
    if (rel_getpixel(0, -1) == Groundtype::GP_SOLID)
      Sound::PingusSound::play_sound("chink");

    WorldObj::get_world()->remove(miner_radius,
                                  static_cast<int>(pingu->get_x() - static_cast<float>((miner_radius.get_width() / 2) + pingu->direction)),
                                                   static_cast<int>(pingu->get_y() - static_cast<float>(miner_radius.get_width() + 1) ));
    pingu->set_action(Actions::WALKER);

    // Stop Pingu walking further into the solid.
    pingu->direction.change();
  }
}

void
Miner::draw (SceneContext& gc)
{
  gc.color().draw(sprite[pingu->direction], pingu->get_pos());
}

} // namespace Actions

/* EOF */
