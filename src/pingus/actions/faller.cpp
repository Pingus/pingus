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

#include "pingus/actions/faller.hpp"

#include "engine/display/scene_context.hpp"
#include "pingus/colliders/pingu_collider.hpp"
#include "pingus/movers/linear_mover.hpp"
#include "pingus/pingu.hpp"
#include "pingus/pingu_enums.hpp"
#include "pingus/world.hpp"
#include "pingus/worldobj.hpp"

namespace Actions {

Faller::Faller (Pingu* p) :
  PinguAction(p),
  faller(),
  tumbler()
{
  faller.load(Direction::LEFT,  Sprite("pingus/player" +
                                       pingu->get_owner_str() + "/faller/left"));
  faller.load(Direction::RIGHT, Sprite("pingus/player" +
                                       pingu->get_owner_str() + "/faller/right"));

  tumbler.load(Direction::LEFT,  Sprite("pingus/player" +
                                        pingu->get_owner_str() + "/tumbler/left"));
  tumbler.load(Direction::RIGHT, Sprite("pingus/player" +
                                        pingu->get_owner_str() + "/tumbler/right"));

  // FIXME: add sprites for jumping here: if x_vel > y_vel, use them
}

Faller::~Faller () { }

void
Faller::update ()
{
  if (is_tumbling())
  {
    tumbler[pingu->direction].update();
  }
  else
  {
    faller[pingu->direction].update();
  }

  // FIXME: This should be triggered at a later point, when close to
  // FIXME: deadly_velocity or something like that. A translation
  // FIXME: animation for the floater might also help
  if (pingu->get_velocity().y > 5.0 && pingu->request_fall_action())
    return;

  // Apply gravity
  pingu->set_velocity(pingu->get_velocity()
                      + Vector3f(0.0f, WorldObj::get_world()->get_gravity()) );

  Vector3f velocity = pingu->get_velocity();
  Vector3f move = velocity;
  bool collided;

  Movers::LinearMover mover(WorldObj::get_world(), pingu->get_pos());

  // Move the Pingu as far is it can go
  mover.update(move, Colliders::PinguCollider(pingu_height));

  pingu->set_pos(mover.get_pos());

  collided = mover.collided();

  // If the Pingu collided with something...
  if (collided)
  {
    if (rel_getpixel(0, -1) == Groundtype::GP_NOTHING &&
        rel_getpixel(0, -2) == Groundtype::GP_NOTHING &&
        rel_getpixel(0, -3) == Groundtype::GP_NOTHING &&
        rel_getpixel(1, 0)  != Groundtype::GP_NOTHING &&
        rel_getpixel(1, 0)  != Groundtype::GP_BRIDGE)
    {
      // Make Pingu bounce off wall
      velocity.x = -(velocity.x / 3.0f);

      // Make the Pingu face the correct direction.  NB: Pingu may
      // previously have been facing in the opposite direction of its
      // velocity because of an explosion.
      if (velocity.x > 0.0f)
        pingu->direction.right();
      else
        pingu->direction.left();

      pingu->set_velocity(velocity);
    }
    else if (velocity.y > 0.0f) // If the Pingu collided into something while moving down...
    {
      // Ping is on ground/water/something
      if (rel_getpixel(0, -1) == Groundtype::GP_WATER
          || rel_getpixel(0, -1) == Groundtype::GP_LAVA)
      {
        pingu->set_action(ActionName::DROWN);
      }
      // Did we stop too fast?
      else if (Math::abs(pingu->get_velocity().y) > deadly_velocity)
      {
        //log_info("Pingus splashed: " << pingu->get_velocity().y << " " << deadly_velocity);
        pingu->set_action(ActionName::SPLASHED);
      }
      else
      {
        // This is where the jumper bug happens
        //log_info("Reached the unreachable: " << pingu->get_velocity().y);
        if (pingu->get_previous_action() == ActionName::BLOCKER)
          pingu->set_action(pingu->get_previous_action());
        else
          pingu->set_action(ActionName::WALKER);
      }
    }
    // If the Pingu collided into something while moving up...
    else // if (velocity.y < 0.0f)
    {
      // Don't make the Pingu go up any further.
      velocity.y = 0;
      velocity.x = (velocity.x / 3.0f);

      pingu->set_velocity(velocity);
    }
  }
}

void
Faller::draw (SceneContext& gc)
{
  if (is_tumbling()) {
    gc.color().draw(tumbler[pingu->direction], pingu->get_pos ());
  } else {
    gc.color().draw(faller[pingu->direction], pingu->get_pos ());
  }
}

bool
Faller::is_tumbling () const
{
  // If we are going fast enough to get smashed, start tumbling
  return (Math::abs(pingu->get_velocity().x) > deadly_velocity
          || Math::abs(pingu->get_velocity().y) > deadly_velocity);
}

bool
Faller::change_allowed (ActionName::Enum new_action)
{
  return
    new_action == ActionName::FLOATER ||
    new_action == ActionName::CLIMBER ||
    new_action == ActionName::BOMBER;
}

} // namespace Actions

/* EOF */
