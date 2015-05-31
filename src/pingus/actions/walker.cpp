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

#include "pingus/actions/walker.hpp"

#include "engine/display/scene_context.hpp"
#include "pingus/globals.hpp"
#include "pingus/groundtype.hpp"
#include "pingus/pingu.hpp"
#include "util/log.hpp"

namespace Actions {

Walker::Walker (Pingu* p) :
  PinguAction(p),
  walker(),
  floaterlayer()
{
  walker.load(Direction::LEFT, "pingus/player" + pingu->get_owner_str() + "/walker/left");
  walker.load(Direction::RIGHT, "pingus/player" + pingu->get_owner_str() + "/walker/right");

  floaterlayer.load(Direction::LEFT,  "other/floaterlayer/left");
  floaterlayer.load(Direction::RIGHT, "other/floaterlayer/right");

  // Reset the velocity
  pingu->set_velocity(Vector3f());
}

void
Walker::update ()
{
  // update the sprite
  walker[pingu->direction].update(0.033f);
  floaterlayer[pingu->direction].update(0.033f);

  Vector3f last_pos = pingu->get_pos();

  /* How should this code work?

     1) Check that the Pingu stands still on ground, if not turn it into
     a faller or drown. The reason we do so, is that we catch situations
     where a digger or a similar action removed the ground under the
     walker.

     2) If pingu is still on ground, we can preprare the next step

     3) Check if up-hill or down-hill is required

     4)

  */

  if (rel_getpixel(1, 0) == Groundtype::GP_OUTOFSCREEN)
  {
    pingu->set_x(pingu->get_x() + static_cast<float>(pingu->direction));
    return;
  }

  if (rel_getpixel(0, -1) ==  Groundtype::GP_WATER)
  {
    pingu->set_action(ActionName::DROWN);
    return;
  }

  // The Pingu stands no longer on ground, the cause for this could be
  // a digger, miner or a bomber
  if (rel_getpixel(0, -1) ==  Groundtype::GP_NOTHING)
  {
    // We search for the nearest ground below the pingu, if we can't
    // find anything within a few pixels, we will turn into a faller
    bool found_ground = false;
    int i;
    for (i = -2; i > -5; --i)
    {
      if (!(rel_getpixel(0, i) == Groundtype::GP_NOTHING))
      {
        found_ground = true;
        break;
      }
    }

    if (found_ground)
    {
      pingu->set_y(pingu->get_y() - static_cast<float>(i));
    }
    else
    {
      pingu->set_action(ActionName::FALLER);
      return;
    }
  }

  // FIXME: here we could/should scan more pixels
  if (rel_getpixel(1, 0) == Groundtype::GP_BRIDGE
      && !head_collision_on_walk(1, 1))  // bridge
  {
    // simple, stupid, but working bridge code
    // FIXME: We don't check if we 'drift' into a solid ground block
    pingu->set_pos(pingu->get_x() + static_cast<float>(pingu->direction),
                   pingu->get_y() - 1.0f); // pingus 'float' through bridges
  }
  else
  {
    // Non of the trivial moves worked, so we do up-hill or down-hill walking
    // FIXME: currently the pingu takes multiple steps at once when
    // FIXME: working uphill, this looks kind of ugly

    // FIXME: rel_getpixel works on the current pos, so modifing pos
    // FIXME: is evil, a backup copy might help

    // if infront is a pixel
    // Pingu is walking up the mountain
    // we can continue walking up. search for the correct y_pos
    int y_inc = 0;
    int possible_y_step = 0;
    bool found_next_step = false;
    for (y_inc = -max_steps; y_inc <= max_steps; ++y_inc)
    {// up/down-hill scan
      if ((  rel_getpixel(1, y_inc)     == Groundtype::GP_NOTHING
             || rel_getpixel(1, y_inc)     == Groundtype::GP_BRIDGE) // FIXME: This causes a rather huge step
          && rel_getpixel(1, y_inc - 1) != Groundtype::GP_NOTHING)
      { // FIXME:
        found_next_step = true;
        possible_y_step = y_inc;
        // No break here, since we always want to use the highest possible position
        //break;
      }
    }

    if (found_next_step)
    {
      // pos.y has a reversed co-system to rel_getpixel()?
      pingu->set_pos(pingu->get_x() + static_cast<float>(pingu->direction),
                     pingu->get_y() - static_cast<float>(possible_y_step));
    }
    else
    {
      if (rel_getpixel(1, 0) !=  Groundtype::GP_NOTHING)
      {
        // We reached a wall
        if (pingu->request_wall_action())
        {
          log_debug("Pingu: We are in front of a wall, setting persistant action");
          return;
        }

        // No persitent action found, so change the direction
        pingu->direction.change();
      }
      else
      {
        // We take the step, so that we are in the air
        pingu->set_x(pingu->get_x() + static_cast<float>(pingu->direction));
        // We reached a cliff
        pingu->set_action(ActionName::FALLER);
        return;
      }
    }
  }

  // This is moved here to fix the bug where pingu stuck turning both
  // sides indefinetely when a head collision occured. the fix needs the
  // above downhill walk being done before head collision check.
  if (head_collision_on_walk(0, 0))
  {
    log_debug("Pingu: Head collision");

    //if the new position causes a head collision, we are already
    //stuck in a wall, so lets go back to the old position
    pingu->direction.change();
    pingu->set_pos(last_pos);
    return;
  }

  /*
    for(int y_inc=1; y_inc <= max_steps; ++y_inc)
    {
    if (rel_getpixel(1, -y_inc) == ColMap::WATER)
    {
    pingu->set_paction ("drown");
    return;
    }
    else if(rel_getpixel(1, -y_inc) != ColMap::NOTHING)
    { // there is land
    pingu->pos.y += y_inc - 1;
    break;
    }
    }
  */
}

void
Walker::draw (SceneContext& gc)
{
  gc.color().draw(walker[pingu->direction], pingu->get_pos());

  if (pingu->get_fall_action() && pingu->get_fall_action()->get_type() == ActionName::FLOATER)
  {
    gc.color().draw(floaterlayer[pingu->direction], pingu->get_pos());
  }
}

} // namespace Actions

/* EOF */
