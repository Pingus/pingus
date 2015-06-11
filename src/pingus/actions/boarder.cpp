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

#include "pingus/actions/boarder.hpp"

#include "engine/display/scene_context.hpp"
#include "pingus/pingu.hpp"

namespace Actions {

Boarder::Boarder (Pingu* p) :
  PinguAction(p),
  x_pos(pingu->get_x()),
  speed(0.0),
  sprite()
{
  sprite.load(Direction::LEFT,  Sprite("pingus/player" +
                                       pingu->get_owner_str() + "/boarder/left"));
  sprite.load(Direction::RIGHT, Sprite("pingus/player" +
                                       pingu->get_owner_str() + "/boarder/right"));
}

void
Boarder::update ()
{
  sprite[pingu->direction].update();

  if (on_ground())
  {
    if (speed < 15.0f)
      speed += 15.0f * 25.0f/1000.0f;
    else {
      speed = 15.0f;
    }

    // Incremental update so that we don't skip pixels
    Vector3f new_pos = pingu->get_pos();
    new_pos.x = new_pos.x + static_cast<float>(pingu->direction) * speed;
    while (new_pos.x != pingu->get_pos().x)
    {
      Vector3f old_pos = pingu->get_pos();
      pingu->set_pos(Vector3f(old_pos.x + ((old_pos.x < new_pos.x) ? 1.0f : -1.0f),
                              old_pos.y,
                              old_pos.z));

      if (pingu->rel_getpixel (1, 0))
      {
        // Hit a wall
        pingu->set_pos(old_pos); // + (pingu->direction * 10);
        ////pingu->pos.y = 10;

        pingu->apply_force(Vector3f(float(speed * static_cast<float>(pingu->direction) * 0.5f),
                                    -float(speed * static_cast<float>(abs(pingu->direction)) * 0.5f)));
        pingu->set_action(ActionName::WALKER);
        return;
      }
    }
  }
  else
  {
    pingu->apply_force (Vector3f(speed * static_cast<float>(pingu->direction), 0));
    pingu->set_action(ActionName::WALKER);
  }
}

void
Boarder::draw (SceneContext& gc)
{
  gc.color().draw(sprite[pingu->direction], pingu->get_pos ());
}

bool
Boarder::on_ground ()
{
  return pingu->rel_getpixel (0, -1) || pingu->rel_getpixel (0, -2);
}

} // namespace Actions

/* EOF */
