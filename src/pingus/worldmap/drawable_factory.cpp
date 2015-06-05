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

#include "pingus/worldmap/drawable_factory.hpp"

#include "pingus/worldmap/sprite_drawable.hpp"
#include "util/log.hpp"
#include "util/mem.hpp"

namespace WorldmapNS {

std::unique_ptr<Drawable>
DrawableFactory::create(const ReaderObject& reader)
{
  if (reader.get_name() == "surface")
  {
    return util::make_unique<SpriteDrawable>(reader.get_mapping());
  }
  else
  {
    log_error("can't create %1%", reader.get_name());
    return {};
  }
}

} // namespace WorldmapNS

/* EOF */
