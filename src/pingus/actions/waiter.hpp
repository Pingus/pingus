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

#ifndef HEADER_PINGUS_PINGUS_ACTIONS_WAITER_HPP
#define HEADER_PINGUS_PINGUS_ACTIONS_WAITER_HPP

#include "engine/display/sprite.hpp"
#include "pingus/animation_clock.hpp"
#include "pingus/pingu_action.hpp"
#include "pingus/pingu_action_view.hpp"

namespace pingus::actions {

/** A Waiting action for the bridger, it gets activated when the
    bridger is out of bridges. It then waits two seconds (meanwhile doing a
    funny animation) and then he changes back to a normal walker. */
class Waiter : public PinguAction
{
  friend class WaiterView;

private:
  float countdown;
  AnimationClock clock;

public:
  Waiter (Pingu*);

  ActionName::Enum get_type() const override { return ActionName::WAITER; }

  void update() override;

private:
  Waiter (Waiter const&);
  Waiter& operator= (Waiter const&);
};

class WaiterView : public PinguActionView
{
private:
  Waiter const& action;
  Sprite sprite;

public:
  WaiterView(Pingu& pingu, Waiter const& action);

  void draw(SceneContext& gc, Pingu& pingu) override;
};

} // namespace pingus::actions

#endif

/* EOF */
