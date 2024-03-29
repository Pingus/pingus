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

#ifndef HEADER_PINGUS_PINGUS_ACTIONS_FLOATER_HPP
#define HEADER_PINGUS_PINGUS_ACTIONS_FLOATER_HPP

#include "engine/display/sprite.hpp"
#include "pingus/pingu_action.hpp"

namespace pingus::actions {

class Floater : public PinguAction
{
private:
  int falling_depth;
  int step;
  Sprite sprite;

public:
  Floater(Pingu* p);

  ActionName::Enum get_type() const override { return ActionName::FLOATER; }

  void init(void);

  void draw (SceneContext& gc) override;
  void update() override;

  char get_persistent_char() override { return 'f'; }
  bool change_allowed (ActionName::Enum new_action) override;

private:
  Floater (Floater const&);
  Floater& operator= (Floater const&);
};

} // namespace pingus::actions

#endif

/* EOF */
