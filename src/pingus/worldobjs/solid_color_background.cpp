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

#include "pingus/worldobjs/solid_color_background.hpp"

#include "engine/display/scene_context.hpp"

namespace pingus::worldobjs {

SolidColorBackground::SolidColorBackground(ReaderMapping const& reader) :
  color()
{
  if (!reader.read("colori", color)) {
    Colorf tmp;
    if (reader.read("color", tmp)) {
      color = tmp.to_color();
    }
  }
}

void
SolidColorBackground::draw (SceneContext& gc)
{
  gc.color().fill_screen(color);
}

} // namespace pingus::worldobjs

/* EOF */
