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

#include "pingus/actions/climber.hpp"

#include "engine/display/scene_context.hpp"
#include "pingus/groundtype.hpp"
#include "pingus/pingu.hpp"

namespace Actions {

Climber::Climber (Pingu* p) :
  PinguAction(p),
  sprite(),
  sprite_width(),
  sprite_height()
{
  sprite.load(Direction::LEFT,  Sprite("pingus/player" +
                                       pingu->get_owner_str() + "/climber/left"));
  sprite.load(Direction::RIGHT, Sprite("pingus/player" +
                                       pingu->get_owner_str() + "/climber/right"));
}

Vector3f
Climber::get_center_pos() const
{
  if (pingu->direction.is_left())
  {
    return pingu->get_pos() + Vector3f(16, 0);
  }
  else
  {
    return pingu->get_pos() + Vector3f(-16, 0);
  }
}

void
Climber::update ()
{
  sprite[pingu->direction].update();

  // If above is free
  if (   rel_getpixel(0, 1) == Groundtype::GP_NOTHING
         || rel_getpixel(0, 1) == Groundtype::GP_BRIDGE)
  {
    // and there is still ground to walk on
    if (rel_getpixel(1, 1) != Groundtype::GP_NOTHING)
    {
      pingu->set_pos(pingu->get_x(),
                     pingu->get_y() - 1);
      return;
    }
    else if (rel_getpixel(1, 1) ==  Groundtype::GP_NOTHING)
    {
      //  log_info("Climber failed, no more wall");

      // If Pingu able to get to new position without head collision
      if (!head_collision_on_walk(pingu->direction, 1))
      {
        // Get ready to walk
        pingu->set_pos(pingu->get_x() + static_cast<float>(pingu->direction),
                       pingu->get_y() - 1.0f);
      }
      else
      {
        // Get ready to fall
        pingu->direction.change();
      }

      // Finish climbing.
      pingu->set_action(ActionName::WALKER);
    }
  }
  else
  {
    //    log_info("Climber failed, falling down");
    pingu->direction.change();
    pingu->set_action(ActionName::WALKER);
  }
}

void
Climber::draw (SceneContext& gc)
{
  gc.color().draw(sprite[pingu->direction], pingu->get_pos());
}

bool
Climber::change_allowed(ActionName::Enum new_action)
{
  return
    (new_action == ActionName::FLOATER) ||
    (new_action == ActionName::BOMBER) ||
    (new_action == ActionName::JUMPER);
}

} // namespace Actions

/* EOF */
