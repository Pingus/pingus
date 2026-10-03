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

#ifndef HEADER_PINGUS_PINGUS_ACTIONS_BOMBER_HPP
#define HEADER_PINGUS_PINGUS_ACTIONS_BOMBER_HPP

#include "pingus/animation_clock.hpp"
#include "pingus/collision_mask.hpp"
#include "pingus/pingu_action.hpp"
#include "pingus/pingu_action_view.hpp"
#include "pingus/state_sprite.hpp"

namespace pingus::actions {

/** An action with lets the Pingu explode. After the explosion the the
    Pingu leaves a hole inside the ground. */
class Bomber : public PinguAction
{
  friend class BomberView;

private:
  bool particle_thrown;
  bool sound_played;
  bool colmap_exploded;

  CollisionMask bomber_radius;
  AnimationClock clock;

public:
  Bomber (Pingu* p);

  ActionName::Enum get_type() const override { return ActionName::BOMBER; }

  bool change_allowed (ActionName::Enum /* action */) override { return false; }

  void update() override;

private:
  Bomber (Bomber const&);
  Bomber& operator= (Bomber const&);
};

class BomberView : public PinguActionView
{
private:
  Bomber const& action;
  StateSprite sprite;
  Sprite explo_surf;

  /** The explosion flash is shown for a single frame */
  bool gfx_exploded;

public:
  BomberView(Pingu& pingu, Bomber const& action);

  void draw(SceneContext& gc, Pingu& pingu) override;
};

} // namespace pingus::actions

#endif

/* EOF */
