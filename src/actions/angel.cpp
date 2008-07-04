//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#include "../display/scene_context.hpp"
#include "../pingu.hpp"
#include "../math.hpp"
#include "../resource.hpp"
#include "../globals.hpp"
#include "angel.hpp"

namespace Actions {

Angel::Angel (Pingu* p)
  : PinguAction(p),
    counter(0.0),
    x_pos(pingu->get_x()),
    sprite(Resource::load_sprite("pingus/player" + pingu->get_owner_str() + "/angel"))
{
}

void
Angel::update ()
{
  sprite.update ();

  counter += game_speed;
  pingu->set_pos((float)x_pos + 20 * (float)Math::sin(counter * 3.0f), pingu->get_y() - 50.0f * 0.025f);

  // Out of screen, let the pingu die
  if (pingu->get_y() < -32)
    pingu->set_status (PS_DEAD);
}

void
Angel::draw (SceneContext& gc)
{
  gc.color().draw (sprite, pingu->get_pos ());
}

} // namespace Actions

/* EOF */
