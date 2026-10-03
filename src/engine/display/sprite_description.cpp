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

#include "math/origin.hpp"
#include "util/reader.hpp"
#include "util/system.hpp"

namespace pingus {

SpriteDescriptionPtr
SpriteDescription::from_file(Pathname const& path)
{
  auto doc = prio::ReaderDocument::from_string(
    System::read_file(path.get_sys_path()), prio::ErrorHandler::THROW, path.str());

  SpriteDescriptionPtr desc = from_mapping(doc.get_root().get_mapping(), path);
  if (desc->filename.empty()) {
    log_error("'image' missing for {}", path.str());
  }
  return desc;
}

SpriteDescriptionPtr
SpriteDescription::from_mapping(prio::ReaderMapping const& reader, Pathname const& context)
{
  SpriteDescriptionPtr desc(new SpriteDescription);
  desc->read(reader, context);
  return desc;
}

void
SpriteDescription::read(prio::ReaderMapping const& reader, Pathname const& context)
{
  reader.read("speed", speed);
  reader.read("loop", loop);
  reader.read("offset", offset);

  reader.read("origin", origin, string2origin);

  Pathname image;
  if (reader.read("image", image))
  {
    // Resolve the image path the same way ResourceManager used to:
    // - historical entries "/images/..." → strip leading slash (datadir-relative)
    // - paths under "images/" are relative to the data directory
    // - bare names like "conveyorbelt_left.png" → relative to the file the
    //   definition comes from, with that file's Pathname type, so sprites
    //   opened from disk (e.g. the sprite viewer) find images next to them
    std::string img = image.get_raw_path();
    if (!img.empty() && img.front() == '/')
      img.erase(img.begin());

    if (img.find("images/") == 0)
    {
      filename = Pathname(img, Pathname::DATA_PATH);
    }
    else
    {
      img = System::normalize_path(Pathname::join(System::dirname(context.get_raw_path()), img));
      filename = Pathname(img, context.get_type());
    }
  }

  reader.read("array", array);
  reader.read("position", frame_pos);
  reader.read("size", frame_size);
}

} // namespace pingus

/* EOF */
