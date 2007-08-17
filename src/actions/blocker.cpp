//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
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

#include "../col_map.hpp"
#include "../display/scene_context.hpp"
#include "../pingu.hpp"
#include "../pingu_holder.hpp"
#include "../world.hpp"
#include "blocker.hpp"

namespace Actions {

Blocker::Blocker(Pingu* p)
  : PinguAction(p)
{
  sprite.load(Direction::LEFT,  "pingus/player" + pingu->get_owner_str() + "/blocker/left");
  sprite.load(Direction::RIGHT, "pingus/player" + pingu->get_owner_str() + "/blocker/right");

  if (   rel_getpixel(0,-1)  ==  Groundtype::GP_NOTHING
      && rel_getpixel(0, -2) ==  Groundtype::GP_GROUND)
    {
      pingu->set_y(pingu->get_y() + 1);
    }
  else if (   rel_getpixel(0,-1) ==  Groundtype::GP_NOTHING
	   && rel_getpixel(0,-2) ==  Groundtype::GP_NOTHING
	   && rel_getpixel(0,-3) ==  Groundtype::GP_GROUND)
    {
      pingu->set_y(pingu->get_y() + 2);
    }
}

void
Blocker::update()
{
  if (!standing_on_ground())
    {
      pingu->set_action(Actions::Faller);
    }
  else
    {
      // FIXME: PinguHolder iterations should be handled otherwise
      PinguHolder* pingus = WorldObj::get_world()->get_pingus();
      for(PinguIter i = pingus->begin(); i != pingus->end(); ++i)
        {
          catch_pingu(*i);
        }
    }
  sprite.update();
}

void
Blocker::draw (SceneContext& gc)
{
  gc.color().draw(sprite[pingu->direction], pingu->get_pos());
}

bool
Blocker::standing_on_ground()
{
  return (rel_getpixel(0,-1) !=  Groundtype::GP_NOTHING);
}

void
Blocker::catch_pingu(Pingu* target)
{
  if (target != pingu) // avoid 'self' catch
    {
      if (target->get_x () > pingu->get_x () - 16
	  && target->get_x () < pingu->get_x () + 16
	  && target->get_y () > pingu->get_y () - 32
	  && target->get_y () < pingu->get_y () + 5
	  )
	{
	  if (target->get_x () > pingu->get_x ()) {
	    target->direction.right();
	  } else {
	    target->direction.left();
	  }
	}
    }
}

int
Blocker::y_offset(void)
{
  return -33;
}

} // namespace Actions

/* EOF */
