// Pingus - A free Lemmings clone
// Copyright (C) 1999 Ingo Ruhnke <grumbel@gmail.com>
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

#include "pingus/worldobjs/liquid.hpp"

#include "engine/display/scene_context.hpp"
#include "pingus/collision_map.hpp"
#include "pingus/world.hpp"

namespace pingus::worldobjs {

Liquid::Liquid(ReaderMapping const& reader) :
  sur(),
  pos(),
  m_z_index(0.0f),
  old_width_handling(false),
  width(0)
{
  ResDescriptor desc;

  InVector2fZ in_vec{pos, m_z_index};
  reader.read("position", in_vec);
  reader.read ("surface",  desc);
  reader.read("repeat",   width);

  sur = Sprite(desc);

  if (!old_width_handling)
    width = width * sur.get_width();
}

float
Liquid::z_index() const
{
  return m_z_index;
}

void
Liquid::on_startup()
{
  CollisionMask mask("liquids/water_cmap");

  for(int i=0; i < width; ++i)
    world->get_colmap()->put(mask,
                             static_cast<int>(pos.x() + static_cast<float>(i)),
                             static_cast<int>(pos.y()),
                             Groundtype::GP_WATER);
}

void
Liquid::draw (SceneContext& gc)
{
  for(int x = static_cast<int>(pos.x());
      x < static_cast<int>(pos.x()) + width;
      x += sur.get_width())
  {
    gc.color().draw(sur, Vector2f(static_cast<float>(x), pos.y()));
  }
}

void
Liquid::update()
{
  sur.update(0.033f);
}

} // namespace pingus::worldobjs

/* EOF */
