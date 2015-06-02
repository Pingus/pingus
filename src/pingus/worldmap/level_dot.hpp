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

#ifndef HEADER_PINGUS_PINGUS_WORLDMAP_LEVEL_DOT_HPP
#define HEADER_PINGUS_PINGUS_WORLDMAP_LEVEL_DOT_HPP

#include "engine/display/sprite.hpp"
#include "pingus/pingus_level.hpp"
#include "pingus/worldmap/dot.hpp"

namespace WorldmapNS {

class LevelDot : public Dot
{
private:
  Sprite green_dot_sur;
  Sprite red_dot_sur;
  Sprite inaccessible_dot_sur;
  Sprite highlight_green_dot_sur;
  Sprite highlight_red_dot_sur;

  PingusLevel plf;

public:
  LevelDot(const ReaderMapping& reader);

  void draw(DrawingContext& gc);
  void draw_hover(DrawingContext& gc);

  void update(float delta);
  PingusLevel get_plf () const { return plf; }
  void on_click();

  bool finished();
  bool accessible();
  void unlock();

private:
  LevelDot (const LevelDot&);
  LevelDot& operator= (const LevelDot&);
};

} // namespace WorldmapNS

#endif

/* EOF */
