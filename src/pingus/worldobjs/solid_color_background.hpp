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

#ifndef HEADER_PINGUS_PINGUS_WORLDOBJS_SOLID_COLOR_BACKGROUND_HPP
#define HEADER_PINGUS_PINGUS_WORLDOBJS_SOLID_COLOR_BACKGROUND_HPP

#include "math/color.hpp"
#include "pingus/worldobj.hpp"

namespace WorldObjsData {
class SolidColorBackgroundData;
}

namespace WorldObjs {

class SolidColorBackground : public WorldObj
{
private:
  Color color;

public:
  SolidColorBackground(const ReaderMapping& reader);

  // FIXME: Make z_position editable
  float get_z_pos () const { return -10; }
  void set_pos(const Vector3f& p) {}
  Vector3f get_pos() const { return Vector3f(); }

  void update () {}

  void draw (SceneContext& gc);

private:
  SolidColorBackground (const SolidColorBackground&);
  SolidColorBackground& operator= (const SolidColorBackground&);
};

} // namespace WorldObjs

#endif

/* EOF */
