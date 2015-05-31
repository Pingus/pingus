// Pingus - A free Lemmings clone
// Copyright (C) 1999 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "pingus/actions/digger.hpp"

#include "engine/display/scene_context.hpp"
#include "engine/sound/sound.hpp"
#include "pingus/pingu.hpp"
#include "pingus/world.hpp"
#include "pingus/worldobj.hpp"

namespace Actions {

Digger::Digger(Pingu* p) :
  PinguAction(p),
  digger_radius("pingus/common/digger_radius_gfx", "pingus/common/digger_radius"),
  digger_radius_final("pingus/common/digger_radius_final_gfx", "pingus/common/digger_radius_final_gfx"),
  sprite(),
  delay_count(0)
{
  sprite = Sprite("pingus/player" + pingu->get_owner_str() + "/digger/left");
}

void
Digger::update()
{
  sprite.update();

  delay_count += 1;

  if (rel_getpixel(0, -1) ==  Groundtype::GP_WATER
      || rel_getpixel(0, -1) ==  Groundtype::GP_LAVA)
  {
    pingu->set_action(ActionName::DROWN);
  }
  else
  {
    if (!have_something_to_dig())
    {
      dig(true);
      pingu->set_action(ActionName::WALKER);
    }
    else if (delay_count % 4 == 0)
    {
      dig(false);
    }
  }
}

bool
Digger::have_something_to_dig()
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
Digger::dig(bool final)
{
  if (!final)
  {
    WorldObj::get_world()->remove(digger_radius,
                                  pingu->get_xi() - digger_radius.get_width() / 2,
                                  pingu->get_yi() - digger_radius.get_height() + 2);
  }
  else
  {
    WorldObj::get_world()->remove(digger_radius_final,
                                  pingu->get_xi() - digger_radius.get_width() / 2,
                                  pingu->get_yi() - digger_radius.get_height() + 2);
  }
  pingu->set_pos(pingu->get_xi(), pingu->get_yi() + 1);
}

void
Digger::draw(SceneContext& gc)
{
  gc.color().draw(sprite, pingu->get_pos());
}

} // namespace Actions

/* EOF */
