// Pingus - A free Lemmings clone
// Copyright (C) 2000 Ingo Ruhnke <grumbel@gmail.com>
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

#include "pingus/actions/slider.hpp"

#include "engine/display/scene_context.hpp"
#include "pingus/groundtype.hpp"
#include "pingus/pingu.hpp"

namespace Actions {

Slider::Slider (Pingu* p) :
  PinguAction(p),
  sprite(),
  speed(10)
{
  sprite.load(Direction::LEFT,  "pingus/player" + pingu->get_owner_str() +
              "/slider/left");
  sprite.load(Direction::RIGHT, "pingus/player" + pingu->get_owner_str() +
              "/slider/right");
}

void
Slider::update ()
{
  sprite[pingu->direction].update();

  for (int i = 0; i < speed && rel_getpixel(1, 0) == Groundtype::GP_NOTHING; ++i)
  {
    pingu->set_x(pingu->get_x() + static_cast<float>(pingu->direction));

    if (rel_getpixel(0, -1) ==  Groundtype::GP_NOTHING)
    {
      speed = (speed > 5) ? 5 : speed;

      if (pingu->direction.is_right())
      {
        pingu->set_velocity(pingu->get_velocity() + Vector3f(speed, 0.0));
      }
      else
      {
        pingu->set_velocity(pingu->get_velocity() + Vector3f(-speed, 0.0));
      }

      pingu->set_action(ActionName::FALLER);
      return;
    }
  }

  speed -= 7 * 0.025f;
  if (speed < 1)
  {
    pingu->set_action(ActionName::WALKER);
    return;
  }
}

void
Slider::draw (SceneContext& gc)
{
  gc.color().draw(sprite[pingu->direction], pingu->get_pos() + Vector3f(0, -2));
}

} // namespace Actions

/* EOF */
