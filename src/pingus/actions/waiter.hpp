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
#include "pingus/pingu_action.hpp"

namespace Actions {

/** A Waiting action for the bridger, it gets activated when the
    bridger is out of bridges. It then waits two seconds (meanwhile doing a
    funny animation) and then he changes back to a normal walker. */
class Waiter : public PinguAction
{
private:
  float countdown;
  Sprite sprite;

public:
  Waiter (Pingu*);

  ActionName::Enum get_type () const { return ActionName::WAITER; }

  void draw (SceneContext& gc);
  void update ();

private:
  Waiter (const Waiter&);
  Waiter& operator= (const Waiter&);
};

} // namespace Actions

#endif

/* EOF */
