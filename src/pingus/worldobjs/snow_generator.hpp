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

#ifndef HEADER_PINGUS_PINGUS_WORLDOBJS_SNOW_GENERATOR_HPP
#define HEADER_PINGUS_PINGUS_WORLDOBJS_SNOW_GENERATOR_HPP

#include "pingus/worldobj.hpp"

namespace WorldObjsData {
class SnowGeneratorData;
}

namespace WorldObjs {

class SnowGenerator : public WorldObj
{
private:
  float intensity;

public:
  SnowGenerator(const ReaderMapping& reader);
  ~SnowGenerator ();

  /** fill the screen with snow flocks */
  void on_startup();
  void draw(SceneContext&) {}
  void update();
  float get_z_pos() const { return 1000; }
  void set_pos(const Vector3f& p) {}
  Vector3f get_pos() const { return Vector3f(); }

private:
  SnowGenerator (const SnowGenerator&);
  SnowGenerator& operator= (const SnowGenerator&);
};

} // namespace WorldObjs

#endif

/* EOF */
