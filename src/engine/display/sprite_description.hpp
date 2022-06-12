// Pingus - A free Lemmings clone
// Copyright (C) 2005 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_ENGINE_DISPLAY_SPRITE_DESCRIPTION_HPP
#define HEADER_PINGUS_ENGINE_DISPLAY_SPRITE_DESCRIPTION_HPP

#include <memory>

#include <geom/point.hpp>
#include <geom/size.hpp>
#include <geom/rect.hpp>
#include <geom/origin.hpp>
#include <prio/reader.hpp>

#include "util/pathname.hpp"

namespace pingus {

class SpriteDescription;

typedef std::shared_ptr<SpriteDescription> SpriteDescriptionPtr;

class SpriteDescription
{
public:
  static SpriteDescriptionPtr from_file(Pathname const& path);

public:
  Pathname    filename;
  geom::ipoint    offset;
  geom::origin      origin;

  bool        loop;
  int         speed;
  geom::isize        array;
  geom::ipoint    frame_pos;
  geom::isize        frame_size;

public:
  SpriteDescription() :
    filename(),
    offset(),
    origin(geom::origin::TOP_LEFT),
    loop(true),
    speed(100),
    array(1, 1),
    frame_pos(),
    frame_size(-1, -1)
  {}

  ~SpriteDescription()
  {
  }
};

} // namespace pingus

#endif

/* EOF */
