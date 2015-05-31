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

#ifndef HEADER_PINGUS_PINGUS_ACTIONS_MINER_HPP
#define HEADER_PINGUS_PINGUS_ACTIONS_MINER_HPP

#include "pingus/collision_mask.hpp"
#include "pingus/pingu_action.hpp"
#include "pingus/state_sprite.hpp"

namespace Actions {

class Miner : public PinguAction
{
private:
  CollisionMask miner_radius;
  CollisionMask miner_radius_left;
  CollisionMask miner_radius_right;

  StateSprite sprite;
  int delay_count;

public:
  Miner (Pingu* p);
  virtual ~Miner () {}

  ActionName::Enum get_type () const { return ActionName::MINER; }

  void draw (SceneContext& gc);
  void update ();

private:
  void mine(bool final);

private:
  Miner (const Miner&);
  Miner& operator= (const Miner&);
};

} // namespace Actions

#endif

/* EOF */
