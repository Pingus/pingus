//  Pingus - A free Lemmings clone
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

#include <cstdio>
#include "display/scene_context.hpp"
#include "pingus/ground_map.hpp"
#include "pingus/world.hpp"
#include "pingus/worldobj.hpp"
#include "sound/sound.hpp"
#include "pingus/pingu.hpp"
#include "actions/digger.hpp"

namespace Actions {

Digger::Digger (Pingu* p) :
  PinguAction(p),
  digger_radius("other/digger_radius_gfx", "other/digger_radius"),
  sprite(),
  digger_c(0)
{
  sprite = Sprite("pingus/player" + pingu->get_owner_str() + "/digger/left");
}

void
Digger::update ()
{
  sprite.update ();

  if (rel_getpixel(0, -1) ==  Groundtype::GP_WATER
      || rel_getpixel(0, -1) ==  Groundtype::GP_LAVA)
    {
      pingu->set_action(Actions::DROWN);
      return;
    }

  if (++digger_c >= 5)
    {
      digger_c = 0;
      dig();
    }

  if (!have_something_to_dig())
    {
      dig ();
      pingu->set_action(Actions::WALKER);
    }
}

bool
Digger::have_something_to_dig ()
{
  if (rel_getpixel(0, -1) !=  Groundtype::GP_NOTHING)
    {
      if (rel_getpixel(0, -1) ==  Groundtype::GP_SOLID)
	{
	  Sound::PingusSound::play_sound("chink");
	  return false;
	}
      else
	return true;
    }
  else
    {
      return false;
    }
}

void
Digger::dig ()
{
  WorldObj::get_world()->remove(digger_radius,
                                static_cast<int>(pingu->get_pos().x - static_cast<float>(digger_radius.get_width() / 2)),
                                static_cast<int>(pingu->get_pos().y - static_cast<float>(digger_radius.get_height()) + 2.0f));
  pingu->set_pos(pingu->get_pos().x, pingu->get_pos().y + 1.0f);
}

void
Digger::draw (SceneContext& gc)
{
  gc.color().draw(sprite, pingu->get_pos ());
}

} // namespace Actions

/* EOF */
