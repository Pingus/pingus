//  $Id: rocket_launcher.cxx,v 1.17 2003/10/22 11:11:23 grumbel Exp $
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

#include "../display/drawing_context.hxx"
//#include "../particles/particle_holder.hxx"
//#include "../particles/explosive_particle.hxx"
#include "../world.hxx"
#include "../resource.hxx"
#include "../pingu.hxx"
#include "../worldobj.hxx"
#include "rocket_launcher.hxx"

namespace Pingus {
namespace Actions {

RocketLauncher::RocketLauncher (Pingu* p)
  : PinguAction(p),
    launched(false)
{
  sprite.load(Direction::LEFT,  "pingus/rocketlauncher/left");
  sprite.load(Direction::RIGHT, "pingus/rocketlauncher/right");

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
RocketLauncher::draw (DrawingContext& gc)
{
  gc.draw(sprite[pingu->direction], pingu->get_pos());
}

} // namespace Actions
} // namespace Pingus

/* EOF */
