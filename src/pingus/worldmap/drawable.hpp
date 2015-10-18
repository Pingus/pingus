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

#ifndef HEADER_PINGUS_PINGUS_WORLDMAP_DRAWABLE_HPP
#define HEADER_PINGUS_PINGUS_WORLDMAP_DRAWABLE_HPP

#include <string>

#include "util/reader.hpp"

class DrawingContext;

namespace WorldmapNS {

/** An drawable object on the worldmap, such has a background or a
    sprite, interface only */
class Drawable
{
protected:
  /** The symbolic id of the drawable */
  std::string name;

  /** True if object should be visible, false otherwise */
  bool visible;

public:
  Drawable() :
    name(),
    visible(true)
  {
  }

  Drawable(std::string arg_name) :
    name(arg_name),
    visible(true)
  {
  }

  Drawable(const ReaderMapping& reader) :
    name(),
    visible(true)
  {
    reader.read_string("name", name);
  }

  virtual ~Drawable() {}

  bool is_visible() { return visible; }
  std::string get_name() { return name; }

  virtual void draw(DrawingContext& gc) =0;
  virtual void update(float delta) =0;

private:
  Drawable (const Drawable&);
  Drawable& operator= (const Drawable&);
};

} // namespace WorldmapNS

#endif

/* EOF */
