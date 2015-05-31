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

#include "pingus/worldobjs/starfield_background_stars.hpp"

#include "engine/display/scene_context.hpp"
#include "pingus/globals.hpp"
#include "pingus/world.hpp"

namespace WorldObjs {

StarfieldBackgroundStars::StarfieldBackgroundStars (Type type) :
  sur(),
  x_pos(),
  y_pos(),
  x_add(),
  y_add()
{
  switch (type)
  {
    case SMALL_STAR:
      sur = Sprite("game/stars/small_star");
      break;
    case MIDDLE_STAR:
      sur = Sprite("game/stars/middle_star");
      break;
    case LARGE_STAR:
      sur = Sprite("game/stars/large_star");
      break;
  }

  x_pos = float(rand() % WorldObj::get_world()->get_width());
  y_pos = float(rand() % WorldObj::get_world()->get_height());

  x_add = static_cast<float>(rand() % 5) + 1.0f;
  y_add = 0.0f;
}

void
StarfieldBackgroundStars::update ()
{
  x_pos += x_add;
  y_pos += y_add;

  if (x_pos > WorldObj::get_world()->get_width())
  {
    x_pos = float(-globals::tile_size);
    y_pos = float(rand() % WorldObj::get_world()->get_height());
  }
}

void
StarfieldBackgroundStars::draw (SceneContext& gc)
{
  gc.color().draw(sur, Vector3f (x_pos, y_pos));
}

} // namespace WorldObjs

/* EOF */
