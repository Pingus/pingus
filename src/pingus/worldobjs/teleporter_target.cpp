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

#include "pingus/worldobjs/teleporter_target.hpp"

#include "engine/display/scene_context.hpp"

namespace pingus::worldobjs {

TeleporterTarget::TeleporterTarget(ReaderMapping const& reader) :
  WorldObj(reader),
  pos(),
  m_z_index(0.0f),
  sprite("worldobjs/teleportertarget")
{
  InVector2fZ in_vec{pos, m_z_index};
  reader.read("position", in_vec);
}

float
TeleporterTarget::z_index() const
{
  return m_z_index;
}

void
TeleporterTarget::draw (SceneContext& gc)
{
  gc.color().draw(sprite, pos);
}

void
TeleporterTarget::update()
{
  sprite.update();
}

void
TeleporterTarget::teleporter_used()
{
  sprite.restart();
}

} // namespace pingus::worldobjs

/* EOF */
