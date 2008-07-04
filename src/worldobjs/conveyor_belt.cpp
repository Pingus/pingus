//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#include "../col_map.hpp"
#include "../display/scene_context.hpp"
#include "../pingu.hpp"
#include "../pingu_holder.hpp"
#include "../resource.hpp"
#include "../world.hpp"
#include "conveyor_belt.hpp"

namespace WorldObjs {

ConveyorBelt::ConveyorBelt(const FileReader& reader)
  : left_sur  (Sprite ("worldobjs/conveyorbelt_left")),
    right_sur (Sprite ("worldobjs/conveyorbelt_right")),
    middle_sur(Sprite ("worldobjs/conveyorbelt_middle"))
{
  reader.read_vector("position", pos);
  reader.read_int   ("width",    width);
  reader.read_float ("speed",    speed);
}

void
ConveyorBelt::draw (SceneContext& gc)
{
  gc.color().draw(left_sur, pos);
  for (int i=0; i < width; ++i)
    gc.color().draw(middle_sur,
	    Vector3f(static_cast<float>(pos.x + left_sur.get_width() + i * middle_sur.get_width()),
                  static_cast<float>(pos.y)));

  gc.color().draw(right_sur,
	  Vector3f(static_cast<float>(pos.x + left_sur.get_width() + width * middle_sur.get_width()),
                 static_cast<float>(pos.y)));
}

void
ConveyorBelt::on_startup ()
{
  CollisionMask mask = Resource::load_collision_mask("worldobjs/conveyorbelt_cmap");

  for (int i=0; i < (width + 2); ++i)
    world->put(mask,
               static_cast<int>(pos.x) + (15 * i),
               static_cast<int>(pos.y),
               Groundtype::GP_SOLID);
}

void
ConveyorBelt::update ()
{
	left_sur.update();
	middle_sur.update();
	right_sur.update();

  PinguHolder* holder = world->get_pingus();
  for (PinguIter pingu = holder->begin(); pingu != holder->end(); ++pingu)
    {
      if (   (*pingu)->get_pos().x > pos.x
	  && (*pingu)->get_pos().x < pos.x + 15 * (width + 2)
	  && (*pingu)->get_pos().y > pos.y - 2
	  && (*pingu)->get_pos().y < pos.y + 10)
	{
	  Vector3f pos = (*pingu)->get_pos();
	  pos.x -= speed * 0.025f;
	  (*pingu)->set_pos(pos);
	}
    }
}

float
ConveyorBelt::get_z_pos () const
{
  return pos.z;
}

} // namespace WorldObjs

/* EOF */
