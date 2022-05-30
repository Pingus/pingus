// Pingus - A free Lemmings clone
// Copyright (C) 2002 Ingo Ruhnke <grumbel@gmail.com>
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

#include "pingus/worldmap/dot.hpp"


namespace pingus::worldmap {

Dot::Dot(ReaderMapping const& reader) :
  Drawable(),
  pos(),
  m_z_index(0.0f)
{
  InVector2fZ in_vec{pos, m_z_index};
  reader.read("position", in_vec);
  reader.read("name",     name);

  assert(!name.empty());
}

} // namespace pingus::worldmap

/* EOF */
