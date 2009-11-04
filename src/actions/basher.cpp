//  Pingus - A free Lemmings clone
//  Copyright (C) 1999-2007 Ingo Ruhnke <grumbel@gmx.de>
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

#include "sound/sound.hpp"
#include "pingus/collision_map.hpp"
#include "pingus/debug.hpp"
#include "pingus/globals.hpp"
#include "display/scene_context.hpp"
#include "pingus/ground_map.hpp"
#include "pingus/pingu.hpp"
#include "pingus/world.hpp"
#include "pingus/worldobj.hpp"
#include "actions/basher.hpp"

namespace Actions {

Basher::Basher (Pingu* p) :
  PinguAction(p),
  sprite(),
  bash_radius("other/bash_radius_gfx", "other/bash_radius"),
  basher_c(0),
  first_bash(true),
  bash_radius_width(),
  bash_reach()
{
  sprite.load(Direction::LEFT,  Sprite("pingus/player" + 
                                       pingu->get_owner_str() + "/basher/left"));
  sprite.load(Direction::RIGHT, Sprite("pingus/player" + 
                                       pingu->get_owner_str() + "/basher/right"));

  bash_radius_width = bash_radius.get_width();
  
  // The +1 is just in case bash_radius is an odd no.  In which case, want to
  // round up the result.
  bash_reach = static_cast<int>(bash_radius_width + 1) / 2;

  // Start a bash even so the action will stops instantly after the
  // first bash
  bash();
}

void
Basher::draw (SceneContext& gc)
{
  gc.color().draw(sprite[pingu->direction], pingu->get_pos());
}

void
Basher::update ()
{
  sprite[pingu->direction].update();

  ++basher_c;
  if (basher_c % 3 == 0)
  {
    walk_forward();

    // If on walking forward the Basher has now walked on to water or lava
    if (rel_getpixel(0, -1) == Groundtype::GP_WATER
        || rel_getpixel(0, -1) == Groundtype::GP_LAVA)
    {
      pingu->set_action(Actions::DROWN);
    }
    // If walking on to something (i.e. hasn't fallen)
    else if (rel_getpixel(0, -1) != Groundtype::GP_NOTHING)
    {
      // If the Basher has walked into something that it won't be able to
      // bash
      if (rel_getpixel(0, 0) == Groundtype::GP_SOLID
          || rel_getpixel(0, pingu_height) == Groundtype::GP_SOLID)
      {
        // Change direction and let walk code walk forward/up to get out.
        Sound::PingusSound::play_sound("chink");
        pingu->direction.change();
        pingu->set_action(Actions::WALKER);
      }
      else if (have_something_to_dig())
      {
        // We only bash every second step, cause the Pingus would
        // get trapped otherwise in the bashing area.
        if (basher_c % 2 == 0)
          bash();
      }
      else if (sprite[pingu->direction].get_current_frame() // FIXME: Game logic must be separate from Sprite 
               / float(sprite[pingu->direction].get_frame_count()) > 0.6f) 
      { // FIXME: EVIL! Engine must not relay on graphic
        pingu->set_action(Actions::WALKER);
      }
    }
  }
}

void
Basher::bash()
{
  WorldObj::get_world()->remove(bash_radius,
                                static_cast<int>(pingu->get_x () - (bash_radius_width / 2)),
                                static_cast<int>(pingu->get_y () - bash_radius_width + 1));
}

void
Basher::walk_forward()
{
  int y_inc = 0;

  // Find the correct y position to go to next
  for (y_inc = 0; y_inc >= -max_steps_down; --y_inc)
  {
    // If there is something below, get out of this loop
    if (rel_getpixel(0, y_inc - 1) != Groundtype::GP_NOTHING)
      break;
  }

  if (y_inc < -max_steps_down)
  {
    // The step down is too much.  So stop being a Basher and be a Faller.
    pingu->set_action(Actions::FALLER);
  }
  else
  {
    // Note that Pingu::set_pos() is the 'reverse' of the y co-ords of
    // rel_getpixel()
    pingu->set_pos(pingu->get_x() + static_cast<int>(pingu->direction),
                   pingu->get_y() - y_inc);
  }
}

bool
Basher::have_something_to_dig()
{
  if (first_bash)
  {
    first_bash = false;
    return true;
  }

  // Check that there is something "within" the Basher's reach
  for(int x = 0; x <= bash_reach; ++x)
  {
    for (int y = min_bash_height; y <= max_bash_height; ++y)
    {
      if (rel_getpixel(x, y) == Groundtype::GP_GROUND)
      {
        pout(PINGUS_DEBUG_ACTIONS) << "Basher: Found something to dig..." << std::endl;
        return true;
      }
    }
  }

  //std::cout << "nothing to dig found" << std::endl;
  return false;
}

} // namespace Actions

/* EOF */
