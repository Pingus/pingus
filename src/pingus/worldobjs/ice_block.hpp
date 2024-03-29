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

#ifndef HEADER_PINGUS_PINGUS_WORLDOBJS_ICE_BLOCK_HPP
#define HEADER_PINGUS_PINGUS_WORLDOBJS_ICE_BLOCK_HPP

#include "math/vector2f.hpp"
#include "pingus/collision_mask.hpp"
#include "pingus/worldobj.hpp"

namespace pingus::worldobjs {

class IceBlock : public WorldObj
{
private:
  /// The upper/left position  of the iceblock's
  Vector2f pos;
  float m_z_index;

  /** The number of iceblocks, only complete blocks are supported */
  int width;

  float thickness;
  bool  is_finished;
  int   last_contact;
  Sprite block_sur;
  CollisionMask block_sur_cmap;

public:
  IceBlock(ReaderMapping const& reader);

  float z_index() const override { return m_z_index; }
  void set_z_index(float z_index) override { m_z_index = z_index; }
  void set_pos(Vector2f const& p) override { pos = p; }
  Vector2f get_pos() const override { return pos; }

  void on_startup() override;
  void draw (SceneContext& gc) override;
  void update() override;

private:
  IceBlock (IceBlock const&);
  IceBlock& operator= (IceBlock const&);
};

} // namespace pingus::worldobjs

#endif

/* EOF */
