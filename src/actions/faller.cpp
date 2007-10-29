//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include <math.h>
#include "../col_map.hpp"
#include "../colliders/pingu_collider.hpp"
#include "../debug.hpp"
#include "../globals.hpp"
#include "../display/scene_context.hpp"
#include "../movers/linear_mover.hpp"
#include "../pingu.hpp"
#include "../resource.hpp"
#include "../world.hpp"
#include "../worldobj.hpp"
#include "faller.hpp"

namespace Actions {

Faller::Faller (Pingu* p)
  : PinguAction(p)
{
  faller.load(Direction::LEFT,  Resource::load_sprite("pingus/player" + 
                                                      pingu->get_owner_str() + "/faller/left"));
  faller.load(Direction::RIGHT, Resource::load_sprite("pingus/player" + 
                                                      pingu->get_owner_str() + "/faller/right"));

  tumbler.load(Direction::LEFT,  Resource::load_sprite("pingus/player" +
                                                       pingu->get_owner_str() + "/tumbler/left"));
  tumbler.load(Direction::RIGHT, Resource::load_sprite("pingus/player" + 
                                                       pingu->get_owner_str() + "/tumbler/right"));

  // FIXME: add sprites for jumping here: if x_vel > y_vel, use them
}

Faller::~Faller () { }

void
Faller::update ()
{
  tumbler[pingu->direction].update();
  faller[pingu->direction].update();

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
              pingu->set_action(Actions::Drown);
            }
          // Did we stop too fast?
          else if (fabs(pingu->get_velocity().y) > deadly_velocity)
            {
              //std::cout << "Pingus splashed: " << pingu->get_velocity().y << " " << deadly_velocity << std::endl;
              pingu->set_action(Actions::Splashed);
            }
          else
            {
              // This is where the jumper bug happens
              //std::cout << "Reached the unreachable: " << pingu->get_velocity().y << std::endl;
              pingu->set_action(Actions::Walker);
            }
        }
      // If the Pingu collided into something while moving up...
      else // if (velocity.y < 0.0f)
        {
          std::cout << "Head bounce" << std::endl;
          // Don't make the Pingu go up any further.
          velocity.y = 0;

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
  return (   fabs(pingu->get_velocity().x) > deadly_velocity
             || fabs(pingu->get_velocity().y) > deadly_velocity);
}

bool
Faller::change_allowed (ActionName new_action)
{
  return new_action == Actions::Floater || new_action == Actions::Climber;
}

} // namespace Actions

/* EOF */
