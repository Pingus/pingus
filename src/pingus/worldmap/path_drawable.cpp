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

#include "pingus/worldmap/path_drawable.hpp"

#include "engine/display/drawing_context.hpp"

namespace WorldmapNS {

PathDrawable::PathDrawable (const Path& arg_path)
  : path(arg_path)
{
}

void
PathDrawable::draw (DrawingContext& gc)
{
  Path::iterator prev = path.begin();

  for(Path::iterator next = prev + 1; next != path.end(); ++next)
  {
    gc.draw_line(Vector2i(static_cast<int>(prev->x), static_cast<int>(prev->y)),
                 Vector2i(static_cast<int>(next->x), static_cast<int>(next->y)),
                 Color(255, 255, 255));
    prev = next;
  }
}

void
PathDrawable::update (float delta)
{
}

} // namespace WorldmapNS

/* EOF */
