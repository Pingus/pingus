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

#ifndef HEADER_PINGUS_PINGUS_WORLDOBJS_GROUNDPIECE_HPP
#define HEADER_PINGUS_PINGUS_WORLDOBJS_GROUNDPIECE_HPP

#include "math/vector3f.hpp"
#include "pingus/groundtype.hpp"
#include "pingus/res_descriptor.hpp"
#include "pingus/worldobj.hpp"

namespace WorldObjsData {
class GroundpieceData;
}

namespace WorldObjs {

/** Groundpieces form the ground on which the pingus can walk, which
    they can dig and which they can detonate away */
class Groundpiece : public WorldObj
{
private:
  Vector3f        pos;
  ResDescriptor desc;
  Groundtype::GPType gptype;

public:
  Groundpiece(const ReaderMapping& reader);

  float get_z_pos () const { return pos.z; }
  void set_pos(const Vector3f& p) { pos = p; }
  Vector3f get_pos() const { return pos; }

  void draw (SceneContext&) {}
  void on_startup ();
  bool purge_after_startup () { return true; }

private:
  Groundpiece (const Groundpiece&);
  Groundpiece& operator= (const Groundpiece&);
};

} // namespace WorldObjs

#endif

/* EOF */
