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
#include "util/log.hpp"

SpriteDescriptionPtr
SpriteDescription::from_file(const Pathname& path)
{
  ReaderObject reader_object = Reader::parse(path);
  ReaderMapping reader = reader_object.get_mapping();

  SpriteDescriptionPtr desc(new SpriteDescription);

  reader.read_int("speed", desc->speed);
  reader.read_bool("loop", desc->loop);
  reader.read_vector2i("offset", desc->offset);

  reader.read_enum("origin", desc->origin, string2origin);

  if (!reader.read_path("image", desc->filename))
  {
    log_error("'image' missing for %1%", reader_object.get_name());
  }

  desc->filename = Pathname(desc->filename.get_raw_path(), Pathname::DATA_PATH); // FIXME: Hack
  reader.read_size("array", desc->array);
  reader.read_vector2i("position",   desc->frame_pos);
  reader.read_size("size",  desc->frame_size);

  return desc;
}

/* EOF */
