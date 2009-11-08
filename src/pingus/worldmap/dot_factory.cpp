//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#include "pingus/worldmap/dot_factory.hpp"

#include <stdexcept>

#include "pingus/worldmap/level_dot.hpp"

namespace WorldmapNS {

Dot*
DotFactory::create(const FileReader& reader)
{
  if (reader.get_name() == "leveldot")
  {
    return new LevelDot(reader);
  }
  else if (reader.get_name() == "tubedot")
  {
    throw std::runtime_error("DotFactory: unknown tag: ");
  }
  return 0;
}

} // namespace WorldmapNS

/* EOF */
