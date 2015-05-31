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

namespace Actions {

Bomber::Bomber (Pingu* p) :
  PinguAction(p),
  particle_thrown(false),
  sound_played(false),
  gfx_exploded(false),
  colmap_exploded(false),
  bomber_radius("other/bomber_radius_gfx", "other/bomber_radius"),
  sprite(),
  explo_surf(Sprite("pingus/player" + pingu->get_owner_str() + "/explo"))
{
  sprite.load(Direction::LEFT,  "pingus/player" + pingu->get_owner_str() + "/bomber/left");
  sprite.load(Direction::RIGHT, "pingus/player" + pingu->get_owner_str() + "/bomber/right");

  WorldObj::get_world()->play_sound("ohno", pingu->get_pos ());
}

void
Bomber::draw (SceneContext& gc)
{
  if (sprite[pingu->direction].get_current_frame() >= 13 && !gfx_exploded)
  {
    gc.color().draw (explo_surf, Vector3f(pingu->get_x () - 32, pingu->get_y () - 48));
    gfx_exploded = true;
  }

  gc.color().draw(sprite[pingu->direction], pingu->get_pos ());
}

void
Bomber::update ()
{
  sprite.update ();

  Movers::LinearMover mover(WorldObj::get_world(), pingu->get_pos());

  Vector3f velocity = pingu->get_velocity();

  // Move the Pingu
  mover.update(velocity, Colliders::PinguCollider(pingu_height));

  pingu->set_pos(mover.get_pos());

  // If the Bomber hasn't 'exploded' yet and it has hit Water or Lava
  if (sprite[pingu->direction].get_current_frame() <= 9 && (rel_getpixel(0, -1) == Groundtype::GP_WATER
                                                            || rel_getpixel(0, -1) == Groundtype::GP_LAVA))
  {
    pingu->set_action(ActionName::DROWN);
    return;
  }

  // If the Bomber hasn't 'exploded' yet and it has hit the ground too quickly
  if (sprite[pingu->direction].get_current_frame () <= 9 && rel_getpixel(0, -1) != Groundtype::GP_NOTHING
      && velocity.y > deadly_velocity)
  {
    pingu->set_action(ActionName::SPLASHED);
    return;
  }

  if (sprite[pingu->direction].get_current_frame () > 9 && !sound_played) {
    WorldObj::get_world()->play_sound("plop", pingu->get_pos ());
    sound_played = true;
  }

  // Throwing particles
  if (sprite[pingu->direction].get_current_frame () > 12 && !particle_thrown)
  {
    particle_thrown = true;
    WorldObj::get_world()->get_pingu_particle_holder()->add_particle(static_cast<int>(pingu->get_x()),
                                                                     static_cast<int>(pingu->get_y()) - 5);
  }

  if (sprite[pingu->direction].get_current_frame () >= 13 && !colmap_exploded)
  {
    colmap_exploded = true;
    WorldObj::get_world()->remove(bomber_radius,
                                  static_cast<int>(static_cast<int>(pingu->get_x()) - (bomber_radius.get_width()/2)),
                                  static_cast<int>(static_cast<int>(pingu->get_y()) - 16 - (bomber_radius.get_width()/2)));
  }

  // The pingu explode
  if (sprite[pingu->direction].is_finished ())
  {
    pingu->set_status(Pingu::PS_DEAD);
  }
}

} // namespace Actions

/* EOF */
