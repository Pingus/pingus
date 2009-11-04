//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_ACTIONS_BASHER_HPP
#define HEADER_PINGUS_ACTIONS_BASHER_HPP

#include "state_sprite.hpp"
#include "pingu_action.hpp"

namespace Actions {

class Basher : public PinguAction
{
private:
  StateSprite   sprite;
  CollisionMask bash_radius;
  int  basher_c;
  bool first_bash;

  /** Width of the bash_radius surface */
  unsigned int bash_radius_width;

  /** The no. of pixels ahead that a Basher checks for something bashable.
      This is initialised using the size of the bash_radius surface. */
  int bash_reach;

  /** Defines the minimum "wall" height needed for a bash to happen. */
  enum { min_bash_height = 5 };

  /** Defines the maximum height up to which a check is made to see if there
      is anything to bash.  Best to make this at least (min_bash_height +
      pingu_height). */
  enum { max_bash_height = 33 };

  /** Defines the maximum no. of steps down a Basher can go down before it
      stops being a Basher and turns into a Faller. */
  enum { max_steps_down = 3 };

public:
  Basher (Pingu* p);

  ActionName get_type () const { return Actions::BASHER; }

  void draw (SceneContext& gc);
  void update ();

  bool have_something_to_dig ();
  void walk_forward ();
  void bash ();

private:
  Basher (const Basher&);
  Basher& operator= (const Basher&);
};

} // namespace Actions

#endif

/* EOF */
