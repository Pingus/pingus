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

#ifndef HEADER_PINGUS_PINGUS_WORLDOBJS_FAKE_EXIT_HPP
#define HEADER_PINGUS_PINGUS_WORLDOBJS_FAKE_EXIT_HPP

#include "math/vector2f.hpp"
#include "pingus/worldobj.hpp"
#include "fwd.hpp"

namespace pingus::worldobjs {

/** The FakeExit is an object that looks similar to the traditional
    IceExit, but is really a trap that smashed all pingus that try to
    enter it. */
class FakeExit : public WorldObj
{
private:
  Sprite   surface;
  Sprite   smallmap_symbol;

  Vector2f pos;
  float m_z_index;

  bool smashing;

public:
  FakeExit(const ReaderMapping& reader);

  float z_index () const override;
  void set_z_index(float z_index) override { m_z_index = z_index; }
  void set_pos(const Vector2f& p) override { pos = p; }
  Vector2f get_pos() const override { return pos; }

  void draw (SceneContext& gc) override;

  void update () override;

  /** Draws an exit symbol on to the small map. */
  void draw_smallmap(SmallMap* smallmap) override;

private:
  void catch_pingu (Pingu*);

  FakeExit (const FakeExit&);
  FakeExit& operator= (const FakeExit&);
};

} // namespace pingus::worldobjs

#endif

/* EOF */
