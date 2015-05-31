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

#ifndef HEADER_PINGUS_PINGUS_ACTIONS_DIGGER_HPP
#define HEADER_PINGUS_PINGUS_ACTIONS_DIGGER_HPP

#include "pingus/collision_mask.hpp"
#include "engine/display/sprite.hpp"
#include "pingus/pingu_action.hpp"

namespace Actions {

class Digger : public PinguAction
{
private:
  CollisionMask digger_radius;
  CollisionMask digger_radius_final;
  Sprite sprite;
  int delay_count;

public:
  Digger(Pingu*);

  ActionName::Enum get_type() const { return ActionName::DIGGER; }

  bool have_something_to_dig();
  void dig(bool final);

  void draw(SceneContext& gc);
  void update();

private:
  Digger (const Digger&);
  Digger& operator= (const Digger&);
};

} // namespace Actions

#endif

/* EOF */
