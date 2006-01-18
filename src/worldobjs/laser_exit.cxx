//  $Id: laser_exit.cxx,v 1.15 2003/10/20 13:11:09 grumbel Exp $
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

#include "../display/scene_context.hxx"
#include "../pingu.hxx"
#include "../pingu_holder.hxx"
#include "../world.hxx"
#include "laser_exit.hxx"

namespace Pingus {
namespace WorldObjs {

LaserExit::LaserExit(const FileReader& reader)
  : killing(false)
{
  reader.read_vector("position", pos);

  counter.set_size(surface.get_frame_count());
  counter.set_type(GameCounter::once);
  counter.set_speed(5);
  counter = 0;
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
    if (counter.finished()) {
      counter = 0;
      killing = false;
    } else {
      ++counter;
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
} // namespace Pingus

/* EOF */
