//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_PINGUS_ACTIONS_ROCKET_LAUNCHER_HPP
#define HEADER_PINGUS_ACTIONS_ROCKET_LAUNCHER_HPP

#include "../state_sprite.hpp"
#include "../pingu_action.hpp"

namespace Actions {

class RocketLauncher : public PinguAction
{
private:
  StateSprite sprite;
  bool launched;
public:
  RocketLauncher(Pingu*);
  ~RocketLauncher() {}

  ActionName get_type () const { return Actions::ROCKETLAUNCHER; }

  void draw (SceneContext& gc);
  void update ();

private:
  RocketLauncher (const RocketLauncher&);
  RocketLauncher& operator= (const RocketLauncher&);
};

} // namespace Actions

#endif

/* EOF */
