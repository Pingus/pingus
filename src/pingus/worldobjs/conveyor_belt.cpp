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

#include "pingus/worldobjs/conveyor_belt.hpp"

#include <logmich/log.hpp>

#include "engine/display/scene_context.hpp"
#include "pingus/pingu.hpp"
#include "pingus/pingu_holder.hpp"
#include "pingus/world.hpp"

namespace pingus::worldobjs {

ConveyorBelt::ConveyorBelt(ReaderMapping const& reader) :
  left_sur  (Sprite ("worldobjs/conveyorbelt_left")),
  right_sur (Sprite ("worldobjs/conveyorbelt_right")),
  middle_sur(Sprite ("worldobjs/conveyorbelt_middle")),
  pos(),
  m_z_index(0.0f),
  width(),
  speed(),
  counter()
{
  InVector2fZ in_vec{pos, m_z_index};
  reader.read("position", in_vec);
  if (!reader.read("repeat",    width))
  {
    log_warn("old 'width' tag used");
    reader.read("width",    width);
  }
  reader.read("speed",    speed);
}

void
ConveyorBelt::draw (SceneContext& gc)
{
  gc.color().draw(left_sur, pos);
  for (int i=0; i < width; ++i)
    gc.color().draw(middle_sur,
                    Vector2f(pos.x() + static_cast<float>(left_sur.get_width() + i * middle_sur.get_width()),
                             pos.y()),
                    m_z_index);

  gc.color().draw(right_sur,
                  Vector2f(pos.x() + static_cast<float>(left_sur.get_width() + width * middle_sur.get_width()),
                           pos.y()),
                  m_z_index);
}

void
ConveyorBelt::on_startup()
{
  CollisionMask mask("worldobjs/conveyorbelt_cmap");

  for (int i=0; i < (width + 2); ++i)
    world->put(mask,
               static_cast<int>(pos.x()) + (15 * i),
               static_cast<int>(pos.y()),
               Groundtype::GP_SOLID);
}

void
ConveyorBelt::update()
{
  left_sur.update();
  middle_sur.update();
  right_sur.update();

  PinguHolder* holder = world->get_pingus();
  for (PinguIter pingu = holder->begin(); pingu != holder->end(); ++pingu)
  {
    if (   (*pingu)->get_pos().x() > pos.x()
           && (*pingu)->get_pos().x() < pos.x() + 15 * static_cast<float>(width + 2)
           && (*pingu)->get_pos().y() > pos.y() - 2
           && (*pingu)->get_pos().y() < pos.y() + 10)
    {
      (*pingu)->set_pos(Vector2f((*pingu)->get_pos().x() - speed * 0.025f,
                                 (*pingu)->get_pos().y()));
    }
  }
}

float
ConveyorBelt::z_index() const
{
  return m_z_index;
}

} // namespace pingus::worldobjs

/* EOF */
