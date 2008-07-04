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
//#include "../particles/particle_holder.hpp"
//#include "../particles/explosive_particle.hpp"
#include "../world.hpp"
#include "../resource.hpp"
#include "../pingu.hpp"
#include "../worldobj.hpp"
#include "rocket_launcher.hpp"

namespace Actions {

RocketLauncher::RocketLauncher (Pingu* p)
  : PinguAction(p),
    launched(false)
{
  sprite.load(Direction::LEFT,  "pingus/player" + pingu->get_owner_str() + 
    "/rocketlauncher/left");
  sprite.load(Direction::RIGHT, "pingus/player" + pingu->get_owner_str() + 
    "/rocketlauncher/right");

/* Explosive Particles not supported right now
  WorldObj::get_world()->get_particle_holder()->add_particle
    (new ExplosiveParticle (static_cast<int>(pingu->get_x()),
			    static_cast<int>(pingu->get_y()) - 12,
			    pingu->direction.is_left() ? -400.0f : 400.0f,
			    0.0f));
*/
}

void
RocketLauncher::update ()
{
  if (sprite[pingu->direction].is_finished())
    {
      pingu->set_action(Actions::Walker);
    }

  sprite.update();
}

void
RocketLauncher::draw (SceneContext& gc)
{
  gc.color().draw(sprite[pingu->direction], pingu->get_pos());
}

} // namespace Actions

/* EOF */
