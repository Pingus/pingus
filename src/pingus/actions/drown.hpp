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

#ifndef HEADER_PINGUS_PINGUS_ACTIONS_DROWN_HPP
#define HEADER_PINGUS_PINGUS_ACTIONS_DROWN_HPP

#include "pingus/animation_clock.hpp"
#include "pingus/pingu_action.hpp"
#include "pingus/pingu_action_view.hpp"
#include "pingus/state_sprite.hpp"

namespace pingus::actions {

class Drown : public PinguAction
{
  friend class DrownView;

private:
  AnimationClock clock;

public:
  Drown (Pingu* p);

  ActionName::Enum get_type() const override { return ActionName::DROWN; }

  void update() override;

  bool catchable() override { return false; }

private:
  Drown (Drown const&);
  Drown& operator= (Drown const&);
};

class DrownView : public PinguActionView
{
private:
  Drown const& action;
  StateSprite sprite;

public:
  DrownView(Pingu& pingu, Drown const& action);

  void draw(SceneContext& gc, Pingu& pingu) override;
};

} // namespace pingus::actions

#endif

/* EOF */
