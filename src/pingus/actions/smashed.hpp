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

#ifndef HEADER_PINGUS_PINGUS_ACTIONS_SMASHED_HPP
#define HEADER_PINGUS_PINGUS_ACTIONS_SMASHED_HPP

#include "engine/display/sprite.hpp"
#include "pingus/pingu_action.hpp"

namespace Actions {

/** FIXME: this action doesn't have a purpose, its pretty much equal
    to the new splashed action */
class Smashed : public PinguAction
{
private:
  bool sound_played;
  Sprite sprite;

public:
  Smashed (Pingu*);

  ActionName::Enum get_type () const { return ActionName::SMASHED; }

  void draw (SceneContext& gc);
  void update ();

  bool catchable () { return false; }

private:
  Smashed (const Smashed&);
  Smashed& operator= (const Smashed&);
};

} // namespace Actions

#endif

/* EOF */
