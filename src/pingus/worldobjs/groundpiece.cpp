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

#include "pingus/worldobjs/groundpiece.hpp"

#include "pingus/world.hpp"

namespace WorldObjs {

Groundpiece::Groundpiece(const ReaderMapping& reader) :
  pos(),
  desc(),
  gptype()
{
  reader.read_vector("position", pos);
  reader.read_desc  ("surface",  desc);

  gptype = Groundtype::GP_GROUND;
  reader.read_enum("type", gptype, &Groundtype::string_to_type);
}

void
Groundpiece::on_startup ()
{
  // FIXME: using a CollisionMask is kind of unneeded here
  CollisionMask mask(desc);

  // FIXME: overdrawing of bridges and similar things aren't handled here
  if (gptype == Groundtype::GP_REMOVE)
    get_world()->remove(mask, static_cast<int>(pos.x), static_cast<int>(pos.y));
  else
    get_world()->put(mask, static_cast<int>(pos.x), static_cast<int>(pos.y), gptype);
}

} // namespace WorldObjs

/* EOF */
