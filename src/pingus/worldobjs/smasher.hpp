// Pingus - A free Lemmings clone
// Copyright (C) 1999 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_PINGUS_WORLDOBJS_SMASHER_HPP
#define HEADER_PINGUS_PINGUS_WORLDOBJS_SMASHER_HPP

#include "math/vector2f.hpp"
#include "pingus/worldobj.hpp"

namespace pingus::worldobjsData {
class SmasherData;
}

class Pingu;

namespace pingus::worldobjs {

class Smasher : public WorldObj
{
private:
  Sprite  sprite;
  Vector2f pos;
  float m_z_index;
  bool smashing;
  bool downwards;
  int  count;

public:
  Smasher(const ReaderMapping& reader);

  float z_index () const override;
  void set_z_index(float z_index) override { m_z_index = z_index; }
  void set_pos(const Vector2f& p) override { pos = p; }
  Vector2f get_pos() const override { return pos; }

  void draw (SceneContext& gc) override;
  void on_startup () override;
  void update () override;

protected:
  void catch_pingu (Pingu* pingu);

private:
  Smasher (const Smasher&);
  Smasher& operator= (const Smasher&);
};

} // namespace pingus::worldobjs

#endif

/* EOF */
