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

#ifndef HEADER_PINGUS_PINGUS_WORLDOBJS_ENTRANCE_HPP
#define HEADER_PINGUS_PINGUS_WORLDOBJS_ENTRANCE_HPP

#include "math/vector2f.hpp"
#include "pingus/worldobj.hpp"

namespace WorldObjsData {
class EntranceData;
}

class Pingu;

namespace WorldObjs {

/** A abstract representation of an entrance, the implementation
    currently sucks and needs to be rewritten */
class Entrance : public WorldObj
{
public:
  enum EntranceDirection { LEFT, RIGHT, MISC };

protected:
  EntranceDirection direction;
  Vector2f          pos;
  float m_z_index;
  int               release_rate;
  int               owner_id;

  Sprite smallmap_symbol;
  Sprite surface;
  int    last_release;
  int    last_direction;

public:
  Entrance(const ReaderMapping& reader);
  ~Entrance() override;

  float z_index () const override;
  void set_z_index(float z_index) override { m_z_index = z_index; }
  void set_pos(const Vector2f& p) override { pos = p; }
  Vector2f get_pos() const override { return pos; }

  virtual bool   pingu_ready ();
  virtual void   create_pingu ();

  void   update () override;

  void   draw (SceneContext& gc) override;

  void  draw_smallmap(SmallMap* smallmap) override;

  int get_owner_id() const { return owner_id; }
private:
  Entrance (const Entrance&);
  Entrance& operator= (const Entrance&);
};

} // namespace WorldObjs

#endif

/* EOF */
