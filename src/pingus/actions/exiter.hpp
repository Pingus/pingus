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

#ifndef HEADER_PINGUS_PINGUS_ACTIONS_EXITER_HPP
#define HEADER_PINGUS_PINGUS_ACTIONS_EXITER_HPP

#include "pingus/animation_clock.hpp"
#include "pingus/pingu_action.hpp"
#include "pingus/pingu_action_view.hpp"
#include "pingus/state_sprite.hpp"

namespace pingus::actions {

class Exiter : public PinguAction
{
  friend class ExiterView;

private:
  AnimationClock clock;
  bool sound_played;

public:
  Exiter(Pingu*);
  void init(void);
  ActionName::Enum get_type() const override { return ActionName::EXITER; }

  void update() override;

private:
  Exiter (Exiter const&);
  Exiter& operator= (Exiter const&);
};

class ExiterView : public PinguActionView
{
private:
  Exiter const& action;
  StateSprite sprite;

public:
  ExiterView(Pingu& pingu, Exiter const& action);

  void draw(SceneContext& gc, Pingu& pingu) override;
};

} // namespace pingus::actions

#endif

/* EOF */
