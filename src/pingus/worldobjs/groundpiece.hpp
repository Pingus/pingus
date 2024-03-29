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

#include "math/vector2f.hpp"
#include "pingus/groundtype.hpp"
#include "pingus/res_descriptor.hpp"
#include "pingus/worldobj.hpp"

namespace pingus::worldobjs {

/** Groundpieces form the ground on which the pingus can walk, which
    they can dig and which they can detonate away */
class Groundpiece : public WorldObj
{
private:
  Vector2f pos;
  float m_z_index;
  ResDescriptor desc;
  Groundtype::GPType gptype;

public:
  Groundpiece(ReaderMapping const& reader);

  float z_index() const override { return m_z_index; }
  void set_z_index(float z_index) override { m_z_index = z_index; }
  void set_pos(Vector2f const& p) override { pos = p; }
  Vector2f get_pos() const override { return pos; }

  void draw (SceneContext&) override {}
  void on_startup() override;
  bool purge_after_startup() override { return true; }

private:
  Groundpiece (Groundpiece const&);
  Groundpiece& operator= (Groundpiece const&);
};

} // namespace pingus::worldobjs

#endif

/* EOF */
