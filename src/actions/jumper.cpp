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

#include "../math/vector3f.hpp"
#include "../display/scene_context.hpp"
#include "../pingu.hpp"
#include "../resource.hpp"
#include "jumper.hpp"

namespace Actions {

Jumper::Jumper (Pingu* p)
  : PinguAction(p)
{
  sprite.load(Direction::LEFT,  Resource::load_sprite("pingus/player" + 
                                                      pingu->get_owner_str() + "/jumper/left"));
  sprite.load(Direction::RIGHT, Resource::load_sprite("pingus/player" + 
                                                      pingu->get_owner_str() + "/jumper/right"));
}

void
Jumper::draw (SceneContext& gc)
{
  gc.color().draw(sprite[pingu->direction], pingu->get_pos());
}

void
Jumper::update ()
{
  // if climber, do a wall-jump, else just jump forward
  if ((pingu->get_previous_action() == Actions::Climber))
    pingu->direction.change();
  
  if (pingu->direction.is_left())
    {
      pingu->set_velocity(pingu->get_velocity() + Vector3f(-5.0, -5.0));
    }
  else // if (pingu->direction.is_right())
    {
      pingu->set_velocity(pingu->get_velocity() + Vector3f(5.0, -5.0));
    }

  // Move the pingu in the air, so that it can start 'falling'
  pingu->set_y(pingu->get_y() - 1);

  pingu->set_action (Actions::Faller);
}

} // namespace Actions

/* EOF */
