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

#ifndef HEADER_PINGUS_PINGUS_WORLDMAP_DOT_HPP
#define HEADER_PINGUS_PINGUS_WORLDMAP_DOT_HPP

#include "math/vector3f.hpp"
#include "pingus/worldmap/drawable.hpp"

namespace WorldmapNS {

/** A Dot is a node between all the pathes on the worldmap, there are
    LevelDots TubeDots and other availabe. */
class Dot : public Drawable
{
protected:
  Vector3f pos;

public:
  Dot(const ReaderMapping& reader);

  /** Draw stuff that should be displayed if the mouse is over the dot */
  virtual void draw_hover(DrawingContext& gc) =0;

  Vector3f get_pos() { return pos; }

  virtual void on_click() =0;

  virtual bool finished() =0;
  virtual bool accessible() =0;
  /** makes the node accessible */
  virtual void unlock() =0;
private:
  Dot (const Dot&);
  Dot& operator= (const Dot&);
};

} // namespace WorldmapNS

#endif

/* EOF */
