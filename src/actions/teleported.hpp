// Pingus - A free Lemmings clone
// Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef HEADER_PINGUS_ACTIONS_TELEPORTED_HXX
#define HEADER_PINGUS_ACTIONS_TELEPORTED_HXX

#include "../pingu_action.hpp"

namespace Actions {

class Teleported : public PinguAction
{
private:
  //bool particle_thrown;
  bool sound_played;
  Sprite sprite;

public:
  Teleported (Pingu*);

  ActionName get_type() const { return Actions::Teleported; }

  void draw (SceneContext& gc);
  void update();

  bool catchable () { return false; }

  int x_target, y_target; // <- FIXME: Ugly!

private:
  Teleported (const Teleported&);
  Teleported& operator= (const Teleported&);
};

} // namespace Actions

#endif

/* EOF */
