//  $Id: laser_exit.cpp 2986 2007-08-17 16:20:09Z grumbel $
//
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

#include "../display/scene_context.hpp"
#include "../pingu.hpp"
#include "../pingu_holder.hpp"
#include "../world.hpp"
#include "laser_exit.hpp"

namespace WorldObjs {

LaserExit::LaserExit(const FileReader& reader)
  : killing(false)
{
  reader.read_vector("position", pos);
}

float
LaserExit::get_z_pos () const
{
  return pos.z;
}

void
LaserExit::draw (SceneContext& gc)
{
  gc.color().draw (surface, pos);
}

void
LaserExit::update ()
{

  PinguHolder* holder = world->get_pingus();
  for (PinguIter pingu = holder->begin (); pingu != holder->end (); ++pingu){
    catch_pingu(*pingu);
  }

  if (killing) {
    if (surface.is_finished()) {
			surface.restart();
      killing = false;
    } else {
      surface.update();
    }
  }
}

void
LaserExit::catch_pingu (Pingu* pingu)
{
  if (!killing)
    {
      if (   pingu->get_x () < pos.x + 34 + 10 && pingu->get_x () > pos.x + 34
	     && pingu->get_y () < pos.y + 43 + 20 && pingu->get_y () > pos.y + 43)
	{
	  if (pingu->get_action() != Actions::Laserkill)
	    {
	      killing = true;
	      pingu->set_action(Actions::Laserkill);
	    }
	}
    }
}

} // namespace WorldObjs

/* EOF */
