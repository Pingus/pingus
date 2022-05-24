// Pingus - A free Lemmings clone
// Copyright (C) 1998-2011 Ingo Ruhnke <grumbel@gmail.com>
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

#include "engine/display/sprite_description.hpp"

#include <logmich/log.hpp>

namespace pingus {

SpriteDescriptionPtr
SpriteDescription::from_file(const Pathname& path)
{
  auto doc = ReaderDocument::from_file(path.get_sys_path());
  ReaderMapping reader = doc.get_root().get_mapping();

  SpriteDescriptionPtr desc(new SpriteDescription);

  reader.read("speed", desc->speed);
  reader.read("loop", desc->loop);
  reader.read("offset", desc->offset);

  reader.read("origin", desc->origin, string2origin);

  if (!reader.read("image", desc->filename))
  {
    log_error("'image' missing for {}", doc.get_root().get_name());
  }

  desc->filename = Pathname(desc->filename.get_raw_path(), Pathname::DATA_PATH); // FIXME: Hack
  reader.read("array", desc->array);
  reader.read("position",   desc->frame_pos);
  reader.read("size",  desc->frame_size);

  return desc;
}

} // namespace pingus

/* EOF */
