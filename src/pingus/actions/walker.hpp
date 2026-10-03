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

#ifndef HEADER_PINGUS_PINGUS_ACTIONS_WALKER_HPP
#define HEADER_PINGUS_PINGUS_ACTIONS_WALKER_HPP

#include "pingus/animation_clock.hpp"
#include "pingus/pingu_action.hpp"
#include "pingus/pingu_action_view.hpp"
#include "pingus/state_sprite.hpp"

namespace pingus::actions {

class Walker : public PinguAction
{
  friend class WalkerView;

private:
  DirectionalAnimationClock walker_clock;
  DirectionalAnimationClock floaterlayer_clock;

  enum { max_steps = 5 }; // max nr. of pixels that pingu can walk up/down

public:
  Walker (Pingu*);

  void update() override;

  ActionName::Enum get_type() const override { return ActionName::WALKER; }

private:
  Walker (Walker const&);
  Walker& operator= (Walker const&);
};

class WalkerView : public PinguActionView
{
private:
  Walker const& action;
  StateSprite walker;
  StateSprite floaterlayer;

public:
  WalkerView(Pingu& pingu, Walker const& action);

  void draw(SceneContext& gc, Pingu& pingu) override;
};

} // namespace pingus::actions

#endif

/* EOF */
