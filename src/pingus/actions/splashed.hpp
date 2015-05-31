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

#ifndef HEADER_PINGUS_PINGUS_ACTIONS_SPLASHED_HPP
#define HEADER_PINGUS_PINGUS_ACTIONS_SPLASHED_HPP

#include "engine/display/sprite.hpp"
#include "pingus/pingu_action.hpp"

namespace Actions {

class Splashed : public PinguAction
{
private:
  bool particle_thrown;
  bool sound_played;
  Sprite sprite;

public:
  Splashed (Pingu*);

  ActionName::Enum get_type () const { return ActionName::SPLASHED; }

  void draw (SceneContext& gc);
  void update ();

  bool catchable () { return false; }
  bool change_allowed (ActionName::Enum ) { return false; }

private:
  Splashed (const Splashed&);
  Splashed& operator= (const Splashed&);
};

} // namespace Actions

#endif

/* EOF */
