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

#ifndef HEADER_PINGUS_PINGUS_WORLDOBJS_STARFIELD_BACKGROUND_HPP
#define HEADER_PINGUS_PINGUS_WORLDOBJS_STARFIELD_BACKGROUND_HPP

#include <vector>

#include "pingus/worldobj.hpp"

namespace WorldObjsData {
class StarfieldBackgroundData;
}

namespace WorldObjs {

class StarfieldBackgroundStars;

class StarfieldBackground : public WorldObj
{
private:
  int  small_stars_count;
  int middle_stars_count;
  int  large_stars_count;

  std::vector<StarfieldBackgroundStars*>        stars;

public:
  StarfieldBackground(const ReaderMapping& reader);
  ~StarfieldBackground();

  // FIXME: Make z_pos handling editable via xml
  float get_z_pos() const { return -10; }
  void set_pos(const Vector3f& p) {}
  Vector3f get_pos() const { return Vector3f(); }

  void update ();
  void draw (SceneContext& gc);

private:
  StarfieldBackground (const StarfieldBackground&);
  StarfieldBackground& operator= (const StarfieldBackground&);
};

} // namespace WorldObjs

#endif

/* EOF */
