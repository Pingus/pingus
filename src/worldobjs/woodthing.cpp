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

#include "math/math.hpp"
#include "display/scene_context.hpp"
#include "particles/smoke_particle_holder.hpp"
#include "resource.hpp"
#include "world.hpp"
#include "worldobjs/woodthing.hpp"

namespace WorldObjs {
// FIXME: Re-enable this namespace.
// namespace Entrances {

WoodThing::WoodThing(const FileReader& reader)
  : Entrance(reader)
{
  surface  = Sprite("entrances/woodthing_mov");
  surface2 = Sprite("entrances/woodthing_nmov");
}

void
WoodThing::update ()
{
  Entrance::update ();
	
	if (last_release > 0)
		surface.update();
	
  if (rand() % 5 == 0)
    {
      world->get_smoke_particle_holder()->
        add_particle(pos.x - surface.get_width () /  2 - 24,
                     pos.y - surface.get_height() + 32 - 147,
                     float(Math::frand() + 1) * -0.6f, float(Math::frand() + 1) * -0.6f);
    }
}

void
WoodThing::draw (SceneContext& gc)
{
	gc.color().draw(surface2, pos);
	// Only draw the animation if a pingu is coming out.
	if (last_release > 0)
		gc.color().draw(surface, pos);
}

// } // namespace Entrances
} // namespace WorldObjs

/* EOF */
