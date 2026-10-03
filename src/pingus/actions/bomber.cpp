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

#include "pingus/actions/bomber.hpp"

#include "engine/display/scene_context.hpp"
#include "pingus/colliders/pingu_collider.hpp"
#include "pingus/movers/linear_mover.hpp"
#include "pingus/particles/pingu_particle_holder.hpp"
#include "pingus/pingu.hpp"
#include "pingus/pingu_enums.hpp"
#include "pingus/world.hpp"

namespace pingus::actions {

Bomber::Bomber (Pingu* p) :
  PinguAction(p),
  colmap_exploded(false),
  bomber_radius("other/bomber_radius_gfx", "other/bomber_radius"),
  // Game timing: 16 steps of 60 ms, the bomber can still drown or splash
  // until step 9 and explodes at step 13
  clock(60, 16, false)
{
}

void
Bomber::update()
{
  clock.update();

  movers::LinearMover mover(pingu->get_world(), pingu->get_pos());

  glm::vec2 velocity = pingu->get_velocity();

  // Move the Pingu
  mover.update(velocity, colliders::PinguCollider(pingu_height));

  pingu->set_pos(mover.get_pos());

  // If the Bomber hasn't 'exploded' yet and it has hit Water or Lava
  if (clock.frame() <= 9 && (rel_getpixel(0, -1) == Groundtype::GP_WATER
                                                            || rel_getpixel(0, -1) == Groundtype::GP_LAVA))
  {
    pingu->set_action(ActionName::DROWN);
    return;
  }

  // If the Bomber hasn't 'exploded' yet and it has hit the ground too quickly
  if (clock.frame() <= 9 && rel_getpixel(0, -1) != Groundtype::GP_NOTHING
      && velocity.y > deadly_velocity)
  {
    pingu->set_action(ActionName::SPLASHED);
    return;
  }

  if (clock.frame() >= 13 && !colmap_exploded)
  {
    colmap_exploded = true;
    pingu->get_world()->remove(bomber_radius,
                                  static_cast<int>(pingu->get_x()) - (bomber_radius.get_width()/2),
                                  static_cast<int>(pingu->get_y()) - 16 - (bomber_radius.get_width()/2));
  }

  // The pingu explode
  if (clock.is_finished())
  {
    pingu->set_status(Pingu::PS_DEAD);
  }
}

void
Bomber::get_look(PinguLook& look) const
{
  // sounds, particles and the explosion flash are effects of the "bomber"
  // animation, see data/animsets/pingus/
  look.add("bomber", clock);
}

} // namespace pingus::actions

/* EOF */
