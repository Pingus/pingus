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

#ifndef HEADER_PINGUS_PINGUS_ACTIONS_CLIMBER_HPP
#define HEADER_PINGUS_PINGUS_ACTIONS_CLIMBER_HPP

#include "pingus/animation_clock.hpp"
#include "pingus/pingu_action.hpp"
#include "pingus/pingu_action_view.hpp"
#include "pingus/state_sprite.hpp"

namespace pingus::actions {

class Climber : public PinguAction
{
  friend class ClimberView;

private:
  DirectionalAnimationClock clock;

public:
  Climber (Pingu*);

  ActionName::Enum get_type() const override { return ActionName::CLIMBER; }


  void update() override;

  char get_persistent_char() override { return 'c'; }
  bool change_allowed(ActionName::Enum new_action) override;

  Vector2f get_center_pos() const override;

private:
  Climber (Climber const&);
  Climber& operator= (Climber const&);
};

class ClimberView : public PinguActionView
{
private:
  Climber const& action;
  StateSprite sprite;

public:
  ClimberView(Pingu& pingu, Climber const& action);

  void draw(SceneContext& gc, Pingu& pingu) override;
};

} // namespace pingus::actions

#endif

/* EOF */
