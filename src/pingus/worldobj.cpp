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

#include "pingus/worldobj.hpp"

namespace pingus {

World* WorldObj::world;

void
WorldObj::set_world(World* arg_world)
{
  world = arg_world;
}

WorldObj::WorldObj(ReaderMapping const& reader) :
  id()
{
  reader.read("id", id);
}

WorldObj::WorldObj() :
  id()
{
  // z_pos = 0;
}

WorldObj::~WorldObj()
{

}

void
WorldObj::on_startup()
{
  // do nothing
}

void
WorldObj::update()
{
  // do nothing
}

void
WorldObj::draw_smallmap(SmallMap* smallmap)
{
}

bool
WorldObj::is_solid_background() const
{
  return false;
}

} // namespace pingus

/* EOF */
