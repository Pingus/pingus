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

#include "pingus/worldobjs/hotspot.hpp"

#include "engine/display/scene_context.hpp"
#include "pingus/res_descriptor.hpp"

namespace pingus::worldobjs {

Hotspot::Hotspot(ReaderMapping const& reader) :
  sprite(),
  para(),
  pos(),
  m_z_index(0.0f)
{
  ResDescriptor desc;

  InVector2fZ in_vec{pos, m_z_index};
  reader.read("position", in_vec);
  reader.read("surface", desc);
  reader.read("parallax", para);

  sprite = Sprite(desc);
}

Hotspot::~Hotspot()
{
}

void
Hotspot::update()
{
  sprite.update();
}

void
Hotspot::draw (SceneContext& gc)
{
  // FIXME: para support doesnn't work correctly
  gc.color().draw (sprite, pos, m_z_index);
}

float
Hotspot::z_index() const
{
  return m_z_index;
}

} // namespace pingus::worldobjs

/* EOF */
