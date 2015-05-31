// Pingus - A free Lemmings clone
// Copyright (C) 2000 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_PINGUS_WORLDOBJS_STARFIELD_BACKGROUND_STARS_HPP
#define HEADER_PINGUS_PINGUS_WORLDOBJS_STARFIELD_BACKGROUND_STARS_HPP

#include "engine/display/sprite.hpp"
#include "pingus/worldobj.hpp"

namespace WorldObjs {

class StarfieldBackgroundStars
{
private:
  Sprite sur;

  float x_pos;
  float y_pos;

  float x_add;
  float y_add;

public:
  enum Type { SMALL_STAR, MIDDLE_STAR, LARGE_STAR };

  StarfieldBackgroundStars() :
    sur(),
    x_pos(),
    y_pos(),
    x_add(),
    y_add()
  {}

  StarfieldBackgroundStars(Type type);

  // Never used
  float get_z_pos () const { return -100; }

  void init ();
  void update ();
  void draw (SceneContext& gc);

private:
  StarfieldBackgroundStars (const StarfieldBackgroundStars&);
  StarfieldBackgroundStars& operator= (const StarfieldBackgroundStars&);
};

} // namespace WorldObjs

#endif

/* EOF */
