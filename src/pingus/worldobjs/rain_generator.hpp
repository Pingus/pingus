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

#ifndef HEADER_PINGUS_PINGUS_WORLDOBJS_RAIN_GENERATOR_HPP
#define HEADER_PINGUS_PINGUS_WORLDOBJS_RAIN_GENERATOR_HPP

#include "pingus/worldobj.hpp"

namespace pingus::worldobjs {

class RainGenerator : public WorldObj
{
private:
  bool  do_thunder;
  float thunder_count;
  float waiter_count;

public:
  RainGenerator(ReaderMapping const& reader);
  ~RainGenerator() override;

  void update() override;
  void draw (SceneContext& gc) override;
  float z_index() const override { return 1000; }
  void set_z_index(float z_index) override {}
  void set_pos(Vector2f const& p) override {}
  Vector2f get_pos() const override { return Vector2f(); }

private:
  RainGenerator (RainGenerator const&);
  RainGenerator& operator= (RainGenerator const&);
};

} // namespace pingus::worldobjs

#endif

/* EOF */
