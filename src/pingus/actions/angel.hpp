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

#ifndef HEADER_PINGUS_PINGUS_ACTIONS_ANGEL_HPP
#define HEADER_PINGUS_PINGUS_ACTIONS_ANGEL_HPP

#include "engine/display/sprite.hpp"
#include "pingus/animation_clock.hpp"
#include "pingus/pingu_action.hpp"
#include "pingus/pingu_action_view.hpp"

namespace pingus::actions {

class Angel : public PinguAction
{
  friend class AngelView;

private:
  float counter;
  float x_pos;
  AnimationClock clock;

public:
  Angel (Pingu* p);

  ActionName::Enum get_type() const override { return ActionName::ANGEL; }

  void  update() override;

private:
  Angel (Angel const&);
  Angel& operator= (Angel const&);
};

class AngelView : public PinguActionView
{
private:
  Angel const& action;
  Sprite sprite;

public:
  AngelView(Pingu& pingu, Angel const& action);

  void draw(SceneContext& gc, Pingu& pingu) override;
};

} // namespace pingus::actions

#endif

/* EOF */
